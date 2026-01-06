#include "taskcommon.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.02
   > GitHub: https://github.com/peihaodong/EventCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

Data::Data()
	:m_strClassName("Data")
{

}

Data::~Data()
{

}

TaskParameter::TaskParameter(uint32_t nTaskType)
	:m_nTaskType(nTaskType)
{

}

TaskParameter::~TaskParameter()
{

}

std::shared_ptr<TaskParameter> TaskParameter::Create(uint32_t nTaskType)
{
	return std::make_shared<TaskParameter>(nTaskType);
}