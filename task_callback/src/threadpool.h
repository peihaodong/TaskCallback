#pragma once
/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.02
   > GitHub: https://github.com/peihaodong/EventCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

#include <thread>
#include <cstdint>
#include <vector>
#include <list>
#include <memory>
#include <functional>
#include <mutex>
#include <atomic>
#include <condition_variable>

//线程池
class ThreadPool
{
public:
	explicit ThreadPool(size_t nThreadNum);
	~ThreadPool();

	static std::shared_ptr<ThreadPool> New(size_t nThreadNum);

	void Submit(const std::function<void(void)>& task);
	
	void Init(size_t nThreadNum);
	void Release();

protected:
	std::list<std::function<void(void)>> m_tasks;//任务队列
	std::vector<std::shared_ptr<std::thread>> m_threads;//线程集合
	std::mutex m_mutex;
	std::condition_variable m_condition;
	std::atomic_bool m_exit = false;//是否退出
};
using PThreadPool = std::shared_ptr<ThreadPool>;

//线程池_指定
class ThreadPool_Specify
{
public:
	struct SThreadInfo
	{
		static std::shared_ptr<SThreadInfo> New();

		std::list<std::function<void(void)>> m_tasks;//任务队列
		std::shared_ptr<std::thread> m_thread;//线程
		std::mutex m_mutex;
		std::condition_variable m_condition;
	};
	using PSThreadInfo = std::shared_ptr<SThreadInfo>;

	explicit ThreadPool_Specify(size_t nThreadNum);
	~ThreadPool_Specify();

	static std::shared_ptr<ThreadPool_Specify> New(size_t nThreadNum);

	void Submit(uint32_t nThreadIndex, const std::function<void(void)>& task);

	void Init(size_t nThreadNum);
	void Release();

protected:
	std::vector<PSThreadInfo> m_vecThreadInfo;
	std::mutex m_mutex;
	std::atomic_bool m_exit = false;//是否退出
};
using PThreadPool_Specify = std::shared_ptr<ThreadPool_Specify>;