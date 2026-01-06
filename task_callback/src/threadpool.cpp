#include "threadpool.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.02
   > GitHub: https://github.com/peihaodong/EventCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

ThreadPool::ThreadPool(size_t nThreadNum)
{
	Init(nThreadNum);
}

ThreadPool::~ThreadPool()
{
	Release();
}

std::shared_ptr<ThreadPool> ThreadPool::New(size_t nThreadNum)
{
	return std::make_shared<ThreadPool>(nThreadNum);
}

void ThreadPool::Submit(const std::function<void(void)>& task)
{
	if (m_exit)
		return;

	m_mutex.lock();
	m_tasks.emplace_back(task);
	m_mutex.unlock();

	m_condition.notify_one();
}

void ThreadPool::Init(size_t nThreadNum)
{
	m_exit = false;
	for (size_t i = 0; i < nThreadNum; i++)
	{
		m_threads.emplace_back(std::make_shared<std::thread>([=]() {
			while (true)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				//如果lambda返回值是true，那么wait()直接返回。
				//如果lambda返回值是false，那么wait()将解锁互斥量，并阻塞到本行。
				//		阻塞到其它某个线程调用notify_one()为止。
				//当其它线程用notify_one()将本wait()唤醒后，wait()就不断的尝试重新获取互斥锁，
				//		如果获取不到，那么流程就卡在wait()这里等待获取；
				//		如果获取到了（加锁），继续往下走。
				m_condition.wait(lock, [=]() {
					if (m_exit || !m_tasks.empty())
						return true;
					return false;
				});
				//流程走到这里，这个互斥量一定是被锁着的
				if (m_exit && m_tasks.empty())
					return;
				//从队列中取出task
				std::function<void(void)> task = std::move(m_tasks.front());
				m_tasks.pop_front();
				lock.unlock();
				//执行task
				if (task)
					task();
			}
		}));
	}
}

void ThreadPool::Release()
{
	m_exit = true;
	m_condition.notify_all();
	for (std::shared_ptr<std::thread>& thread : m_threads)
	{
		if (thread->joinable())
			thread->join();
	}
	m_threads.clear();
}

std::shared_ptr<ThreadPool_Specify::SThreadInfo> ThreadPool_Specify::SThreadInfo::New()
{
	return std::make_shared<SThreadInfo>();
}

ThreadPool_Specify::ThreadPool_Specify(size_t nThreadNum)
{
	Init(nThreadNum);
}

ThreadPool_Specify::~ThreadPool_Specify()
{
	Release();
}

std::shared_ptr<ThreadPool_Specify> ThreadPool_Specify::New(size_t nThreadNum)
{
	return std::make_shared<ThreadPool_Specify>(nThreadNum);
}

void ThreadPool_Specify::Submit(uint32_t nThreadIndex, const std::function<void(void)>& task)
{
	if (m_exit)
		return;

	std::unique_lock<std::mutex> lock(m_mutex);

	if (nThreadIndex >= m_vecThreadInfo.size())
		return;

	PSThreadInfo& info = m_vecThreadInfo[nThreadIndex];

	info->m_mutex.lock();
	info->m_tasks.emplace_back(task);
	info->m_mutex.unlock();

	info->m_condition.notify_one();
}

void ThreadPool_Specify::Init(size_t nThreadNum)
{
	m_exit = false;
	std::unique_lock<std::mutex> lock(m_mutex);
	for (size_t i = 0; i < nThreadNum; i++)
	{
		PSThreadInfo info = SThreadInfo::New();
		info->m_thread = std::make_shared<std::thread>([=]() {
			while (true)
			{
				std::unique_lock<std::mutex> lock(info->m_mutex);
				//如果lambda返回值是true，那么wait()直接返回。
				//如果lambda返回值是false，那么wait()将解锁互斥量，并阻塞到本行。
				//		阻塞到其它某个线程调用notify_one()为止。
				//当其它线程用notify_one()将本wait()唤醒后，wait()就不断的尝试重新获取互斥锁，
				//		如果获取不到，那么流程就卡在wait()这里等待获取；
				//		如果获取到了（加锁），继续往下走。
				info->m_condition.wait(lock, [=]() {
					if (m_exit || !info->m_tasks.empty())
						return true;
					return false;
				});
				//流程走到这里，这个互斥量一定是被锁着的
				if (m_exit && info->m_tasks.empty())
					return;
				//从队列中取出task
				std::function<void(void)> task = std::move(info->m_tasks.front());
				info->m_tasks.pop_front();
				lock.unlock();
				//执行task
				if (task)
					task();
			}
		});
		m_vecThreadInfo.push_back(info);
	}
}

void ThreadPool_Specify::Release()
{
	m_exit = true;
	std::unique_lock<std::mutex> lock(m_mutex);
	for (int i = 0; i < m_vecThreadInfo.size(); i++)
	{
		PSThreadInfo& info = m_vecThreadInfo[i];
		info->m_condition.notify_all();
		if (info->m_thread->joinable())
			info->m_thread->join();
	}
	m_vecThreadInfo.clear();
}
