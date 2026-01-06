#include "task.h"
#include <atomic>
#include "threadpool.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

bool Listener::FunctionPointerDescriptor::operator==(const FunctionPointerDescriptor& obj) const
{
	if (m_size != obj.m_size)
		return false;
	if (m_addr && obj.m_addr)
	{
		if (memcmp(m_addr, obj.m_addr, m_size) != 0)
			return false;
	}
	return true;
}

Listener::Listener()
{

}

Listener::~Listener()
{
	if (m_desc.m_addr)
		free(m_desc.m_addr);
}

Listener::Ptr Listener::Create()
{
	return std::make_shared<Listener>();
}

TaskManager::TaskManager(ETaskManagerType type)
{
	m_type = type;

	if (m_type == ETaskManagerType::TMT_ThreadPool)
	{
		m_tp = ThreadPool::New(3);
	}
	else if (m_type == ETaskManagerType::TMT_SpecifyThread)
	{
		m_tp_s = ThreadPool_Specify::New(3);
	}
}

TaskManager::~TaskManager()
{

}

uint64_t TaskManager::Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		iterListeners = (m_mapListeners.insert(std::make_pair(nTaskType, std::vector<Listener::Ptr>()))).first;
	std::vector<Listener::Ptr>& vecListener = iterListeners->second;
	
	//创建监听者并赋值
	Listener::Ptr listener = Listener::Create();
	listener->m_func = callback;
	listener->m_nTaskID = GetUniqueId();
	//判断该指定事件监听队列是否存在相同的监听者
	{
		auto iterListener = std::find_if(vecListener.begin(), vecListener.end(), [&](const Listener::Ptr& obj) {
			return listener->m_nTaskID == obj->m_nTaskID;
		});
		if (iterListener != vecListener.end())
			return 0;
	}

	vecListener.push_back(listener);

	return listener->m_nTaskID;
}

void TaskManager::Cancel(uint32_t nTaskType, uint64_t nTaskID)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		return;
	std::vector<Listener::Ptr>& vecListener = iterListeners->second;

	//创建监听者并赋值
	Listener::Ptr listener = Listener::Create();
	listener->m_nTaskID = nTaskID;
	//找到指定事件监听队列中的监听者，并移除
	auto iterListener = std::find_if(vecListener.begin(), vecListener.end(), [&](const Listener::Ptr& obj) {
		return listener->m_nTaskID == obj->m_nTaskID;
	});
	if (iterListener != vecListener.end())
		vecListener.erase(iterListener);
}

void TaskManager::Cancel(uint32_t nTaskType)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		return;
	m_mapListeners.erase(iterListeners);
}

void TaskManager::Notify(const PTaskParameter& parameter, uint32_t nThreadIndex /*= 0*/)
{
	if (m_type == ETaskManagerType::TMT_UI)
	{
		if (m_funcUINotify)
			m_funcUINotify(parameter);
		return;
	}

	std::unique_lock<std::mutex> lock(m_mutex);

	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListenerQueue = m_mapListeners.find(parameter->m_nTaskType);
	if (iterListenerQueue == m_mapListeners.end())
		return;
	std::vector<Listener::Ptr>& listener_queue = iterListenerQueue->second;
	//执行回调函数
	if (m_type == ETaskManagerType::TMT_ThreadPool)
	{
		m_tp->Submit([=]() {
			for (const Listener::Ptr& listener : listener_queue)
			{
				listener->m_func(parameter);
			}
		});
	}
	else if (m_type == ETaskManagerType::TMT_SpecifyThread)
	{
		m_tp_s->Submit(nThreadIndex, [=]() {
			for (const Listener::Ptr& listener : listener_queue)
			{
				listener->m_func(parameter);
			}
		});
	}
	else
	{
		for (const Listener::Ptr& listener : listener_queue)
		{
			listener->m_func(parameter);
		}
	}
}

void TaskManager::Release()
{
	std::unique_lock<std::mutex> lock(m_mutex);

	if (m_type == ETaskManagerType::TMT_ThreadPool)
	{
		m_tp->Release();
	}
	else if (m_type == ETaskManagerType::TMT_SpecifyThread)
	{
		m_tp_s->Release();
	}

	m_mapListeners.clear();
}

void TaskManager::SetUINotify(const std::function<void(PTaskParameter)>& notify)
{
	m_funcUINotify = notify;
}

void TaskManager::DoUITask(const PTaskParameter& parameter)
{
	std::unique_lock<std::mutex> lock(m_mutex);

	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListenerQueue = m_mapListeners.find(parameter->m_nTaskType);
	if (iterListenerQueue == m_mapListeners.end())
		return;
	std::vector<Listener::Ptr>& listener_queue = iterListenerQueue->second;
	for (const Listener::Ptr& listener : listener_queue)
	{
		listener->m_func(parameter);
	}
}

bool TaskManager::ListenerIsEqual(const Listener::Ptr& listener1, const Listener::Ptr& listener2)
{
	return listener1->m_this == listener2->m_this && listener1->m_desc == listener2->m_desc;
}

uint64_t TaskManager::GetUniqueId() const
{
	static std::atomic<uint64_t> id = 0;
	id++;
	return id;
}
