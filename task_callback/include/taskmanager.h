#pragma once
/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

#include "taskcommon.h"
#include "task.h"

//任务管理者_同步
class Q_DECL_EXPORT TaskManager_Sync
{
public:
	~TaskManager_Sync();

	static TaskManager_Sync* GetInstance();

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
	void Notify(const PTaskParameter& parameter);

protected:
	TaskManager_Sync();

protected:
	std::shared_ptr<TaskManager> m_manager;
};

template<typename T>
uint64_t TaskManager_Sync::Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Register(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_Sync::Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	m_manager->Cancel(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_Sync::Cancel(uint32_t nTaskType, T* pThis)
{
	m_manager->Cancel(nTaskType, pThis);
}

//////////////////////////////////////////////////////////////////////////

//任务管理者_线程池
class Q_DECL_EXPORT TaskManager_ThreadPool
{
public:
	~TaskManager_ThreadPool();

	static TaskManager_ThreadPool* GetInstance();

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
	void Notify(const PTaskParameter& parameter);

protected:
	TaskManager_ThreadPool();

protected:
	std::shared_ptr<TaskManager> m_manager;
};

template<typename T>
uint64_t TaskManager_ThreadPool::Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Register(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_ThreadPool::Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	m_manager->Cancel(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_ThreadPool::Cancel(uint32_t nTaskType, T* pThis)
{
	m_manager->Cancel(nTaskType, pThis);
}

//////////////////////////////////////////////////////////////////////////

//任务管理者_指定线程
class Q_DECL_EXPORT TaskManager_SpecifyThread
{
public:
	~TaskManager_SpecifyThread();

	static TaskManager_SpecifyThread* GetInstance();

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
	void Notify(uint32_t nThreadIndex, const PTaskParameter& parameter);

protected:
	TaskManager_SpecifyThread();

protected:
	std::shared_ptr<TaskManager> m_manager;
};

template<typename T>
uint64_t TaskManager_SpecifyThread::Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Register(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_SpecifyThread::Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Cancel(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_SpecifyThread::Cancel(uint32_t nTaskType, T* pThis)
{
	m_manager->Cancel(nTaskType, pThis);
}

//////////////////////////////////////////////////////////////////////////

//任务管理者_UI
class Q_DECL_EXPORT TaskManager_UI
{
public:
	~TaskManager_UI();

	static TaskManager_UI* GetInstance();

	void SetUINotify(const std::function<void(PTaskParameter)>& notify);
	void DoUITask(const PTaskParameter& parameter);

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
	void Notify(const PTaskParameter& parameter);

protected:
	TaskManager_UI();

protected:
	std::shared_ptr<TaskManager> m_manager;
};

template<typename T>
uint64_t TaskManager_UI::Register(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Register(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_UI::Cancel(uint32_t nTaskType, T* pThis, TaskCallback<T> pCallback)
{
	return m_manager->Cancel(nTaskType, pThis, pCallback);
}

template<typename T>
void TaskManager_UI::Cancel(uint32_t nTaskType, T* pThis)
{
	m_manager->Cancel(nTaskType, pThis);
}
