#pragma once
/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

#include "taskcommon.h"
#include <unordered_map>
#include <mutex>

class ThreadPool;
class ThreadPool_Specify;

enum class ETaskManagerType
{
	TMT_Sync,
	TMT_ThreadPool,
	TMT_SpecifyThread,
	TMT_UI
};

//监听者
struct Q_DECL_EXPORT Listener
{
	//函数指针描述者类：用于对比回调函数是否一致，防止多次监听
	struct FunctionPointerDescriptor
	{
		bool operator==(const FunctionPointerDescriptor& obj) const;

		void* m_addr = nullptr;	//函数指针首地址
		size_t m_size = 0;		//函数指针空间大小
	};

	using Ptr = std::shared_ptr<Listener>;

	Listener();
	~Listener();

	static Ptr Create();

	std::function<void(const PTaskParameter&)> m_func;		//回调函数
	FunctionPointerDescriptor m_desc;					//函数指针描述者

	void* m_this = nullptr;								//监听者this指针
	uint64_t m_nTaskID = 0;							//任务ID
};

//任务管理者
class Q_DECL_EXPORT TaskManager
{
public:
	explicit TaskManager(ETaskManagerType type);
	~TaskManager();

	//注册
	template<typename T>
	uint64_t Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback);
	uint64_t Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback);

	//取消
	template<typename T>
	void Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback);
	template<typename T>
	void Cancel(uint32_t nTaskType, T* pThis);
	void Cancel(uint32_t nTaskType, uint64_t nTaskID);
	void Cancel(uint32_t nTaskType);

	//通知
	void Notify(const PTaskParameter& parameter, uint32_t nThreadIndex = 0);

	//释放
	void Release();

	//UI
	void SetUINotify(const std::function<void(PTaskParameter)>& notify);
	void DoUITask(const PTaskParameter& parameter);

protected:
	//创建函数指针描述者（静态模板函数）
	template<typename T>
	static Listener::FunctionPointerDescriptor BuildFunctionPointerDescriptor(TaskCallback<T> pCallback);

	//判断两个监听者是否一致（静态函数）
	static bool ListenerIsEqual(const Listener::Ptr& listener1, const Listener::Ptr& listener2);

	//获取唯一ID
	uint64_t GetUniqueId() const;

protected:
	//一个事件对应多个监听回调函数
	std::unordered_map<uint32_t, std::vector<Listener::Ptr>> m_mapListeners;//key:任务类型 value:监听队列
	std::mutex m_mutex;//对m_mapListeners对象

	ETaskManagerType m_type = ETaskManagerType::TMT_Sync;
	std::shared_ptr<ThreadPool> m_tp;
	std::shared_ptr<ThreadPool_Specify> m_tp_s;

	std::function<void(PTaskParameter)> m_funcUINotify;
};

template<typename T>
uint64_t TaskManager::Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		iterListeners = (m_mapListeners.insert(std::make_pair(nTaskType, std::vector<Listener::Ptr>()))).first;
	std::vector<Listener::Ptr>& vecListener = iterListeners->second;
	//将该回调函数绑定对象
	std::function<void(const PTaskParameter&)> func = std::bind(pCallback, pThis, std::placeholders::_1);
	//创建监听者并赋值
	Listener::Ptr listener = Listener::Create();
	listener->m_func = func;
	listener->m_desc = BuildFunctionPointerDescriptor(pCallback);
	listener->m_this = pThis;
	listener->m_nTaskID = GetUniqueId();
	//判断该指定事件监听队列是否存在相同的监听者
	{
		auto iterListener = std::find_if(vecListener.begin(), vecListener.end(), [&](const Listener::Ptr& obj) {
			return TaskManager::ListenerIsEqual(listener, obj);
		});
		if (iterListener != vecListener.end())
			return 0;
	}
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

template<typename T>
void TaskManager::Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		return;
	std::vector<Listener::Ptr>& vecListener = iterListeners->second;
	//将该回调函数绑定对象
	std::function<void(const PTaskParameter&)> func = std::bind(pCallback, pThis, std::placeholders::_1);
	//创建监听者并赋值
	Listener::Ptr listener = Listener::Create();
	listener->m_this = pThis;
	listener->m_func = func;
	listener->m_desc = BuildFunctionPointerDescriptor(pCallback);
	//找到指定事件监听队列中的监听者，并移除
	auto iterListener = std::find_if(vecListener.begin(), vecListener.end(), [&](const Listener::Ptr& obj) {
		return TaskManager::ListenerIsEqual(listener, obj);
		});
	if (iterListener != vecListener.end())
		vecListener.erase(iterListener);
}

template<typename T>
void TaskManager::Cancel(uint32_t nTaskType, T* pThis)
{
	//在事件监听队列集合中找到指定事件的监听队列
	auto iterListeners = m_mapListeners.find(nTaskType);
	if (iterListeners == m_mapListeners.end())
		return;
	std::vector<Listener::Ptr>& vecListener = iterListeners->second;
	for (auto iter = vecListener.begin(); iter != vecListener.end(); )
	{
		if ((*iter)->m_this == pThis)
			iter = vecListener.erase(iter);
		else
			iter++;
	}
}

template<typename T>
Listener::FunctionPointerDescriptor TaskManager::BuildFunctionPointerDescriptor(TaskCallback<T> pCallback)
{
	size_t s = sizeof(TaskCallback<T>);	//计算该类函数指针大小
	void* p = malloc(s);					//开辟该类函数指针内存
	new(p)TaskCallback<T>(pCallback);		//调用其构造函数

	//创建函数指针描述者，记录其内存地址及大小
	Listener::FunctionPointerDescriptor descriptor;
	descriptor.m_addr = p;
	descriptor.m_size = s;
	return descriptor;
}
