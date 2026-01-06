#pragma once
#include "taskcommon.h"

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
