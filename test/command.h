#pragma once
#include "taskcommon.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

enum ETaskType
{
	TT_NULL,

	TT_Query_UserName,
	TT_Query_UserSex,
	TT_Query_UserAge,

	TT_Reply_UserName,
	TT_Reply_UserSex,
	TT_Reply_UserAge,
};

class StringData : public Data
{
public:
	StringData();

	std::string m_str;
};
