#pragma once
/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.02
   > GitHub: https://github.com/peihaodong/EventCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

#include <QtGlobal>
#include <memory>
#include <string>
#include <functional>
#include <string>

class Q_DECL_EXPORT Data : public std::enable_shared_from_this<Data>
{
public:
	Data();
	virtual ~Data();

protected:
	std::string m_strClassName;
};
using PData = std::shared_ptr<Data>;

struct Q_DECL_EXPORT TaskParameter
{
public:
	explicit TaskParameter(uint32_t nTaskType);
	~TaskParameter();

	static std::shared_ptr<TaskParameter> Create(uint32_t nTaskType);

	uint32_t m_nTaskType = 0;	//任务类型
	PData m_data;				//任务所携带的数据
	void* m_informer = nullptr;	//任务的通知者this指针
};
using PTaskParameter = std::shared_ptr<TaskParameter>;

//类函数指针
template<typename T>
using TaskCallback = void(T::*)(const PTaskParameter&);