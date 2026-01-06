#include "taskmanager.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.02
   > GitHub: https://github.com/peihaodong/EventCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

TaskManager_Sync::~TaskManager_Sync()
{
	m_manager->Release();
}

TaskManager_Sync* TaskManager_Sync::GetInstance()
{
	static TaskManager_Sync inst;
	return &inst;
}

uint64_t TaskManager_Sync::Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback)
{
	return m_manager->Register(nTaskType, callback);
}

void TaskManager_Sync::Cancel(uint32_t nTaskType, uint64_t nTaskID)
{
	m_manager->Cancel(nTaskType, nTaskID);
}

void TaskManager_Sync::Cancel(uint32_t nTaskType)
{
	m_manager->Cancel(nTaskType);
}

void TaskManager_Sync::Notify(const PTaskParameter& parameter)
{
	m_manager->Notify(parameter);
}

TaskManager_Sync::TaskManager_Sync()
{
	m_manager = std::make_shared<TaskManager>(ETaskManagerType::TMT_Sync);
}

TaskManager_ThreadPool::~TaskManager_ThreadPool()
{
	m_manager->Release();
}

TaskManager_ThreadPool* TaskManager_ThreadPool::GetInstance()
{
	static TaskManager_ThreadPool inst;
	return &inst;
}

uint64_t TaskManager_ThreadPool::Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback)
{
	return m_manager->Register(nTaskType, callback);
}

void TaskManager_ThreadPool::Cancel(uint32_t nTaskType, uint64_t nTaskID)
{
	m_manager->Cancel(nTaskType, nTaskID);
}

void TaskManager_ThreadPool::Cancel(uint32_t nTaskType)
{
	m_manager->Cancel(nTaskType);
}

void TaskManager_ThreadPool::Notify(const PTaskParameter& parameter)
{
	m_manager->Notify(parameter);
}

TaskManager_ThreadPool::TaskManager_ThreadPool()
{
	m_manager = std::make_shared<TaskManager>(ETaskManagerType::TMT_ThreadPool);
}

TaskManager_SpecifyThread::~TaskManager_SpecifyThread()
{
	m_manager->Release();
}

TaskManager_SpecifyThread* TaskManager_SpecifyThread::GetInstance()
{
	static TaskManager_SpecifyThread inst;
	return &inst;
}

uint64_t TaskManager_SpecifyThread::Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback)
{
	return m_manager->Register(nTaskType, callback);
}

void TaskManager_SpecifyThread::Cancel(uint32_t nTaskType, uint64_t nTaskID)
{
	m_manager->Cancel(nTaskType, nTaskID);
}

void TaskManager_SpecifyThread::Cancel(uint32_t nTaskType)
{
	m_manager->Cancel(nTaskType);
}

void TaskManager_SpecifyThread::Notify(uint32_t nThreadIndex, const PTaskParameter& parameter)
{
	m_manager->Notify(parameter, nThreadIndex);
}

TaskManager_SpecifyThread::TaskManager_SpecifyThread()
{
	m_manager = std::make_shared<TaskManager>(ETaskManagerType::TMT_SpecifyThread);
}

TaskManager_UI::~TaskManager_UI()
{
	m_manager->Release();
}

TaskManager_UI* TaskManager_UI::GetInstance()
{
	static TaskManager_UI inst;
	return &inst;
}

void TaskManager_UI::SetUINotify(const std::function<void(PTaskParameter)>& notify)
{
	m_manager->SetUINotify(notify);
}

void TaskManager_UI::DoUITask(const PTaskParameter& parameter)
{
	m_manager->DoUITask(parameter);
}

uint64_t TaskManager_UI::Register(uint32_t nTaskType, const std::function<void(const PTaskParameter&)>& callback)
{
	return m_manager->Register(nTaskType, callback);
}

void TaskManager_UI::Cancel(uint32_t nTaskType, uint64_t nTaskID)
{
	m_manager->Cancel(nTaskType, nTaskID);
}

void TaskManager_UI::Cancel(uint32_t nTaskType)
{
	m_manager->Cancel(nTaskType);
}

void TaskManager_UI::Notify(const PTaskParameter& parameter)
{
	m_manager->Notify(parameter);
}

TaskManager_UI::TaskManager_UI()
{
	m_manager = std::make_shared<TaskManager>(ETaskManagerType::TMT_UI);
}
