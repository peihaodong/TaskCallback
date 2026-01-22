#pragma once

#include <QObject>
#include "taskcommon.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

class Module  : public QObject
{
	Q_OBJECT

public:
	Module(QObject *parent = nullptr);
	~Module();

protected:
	void cmdQuertName(const PTaskParameter& parameter);
	void cmdQuertSex(const PTaskParameter& parameter);
	void cmdQuertAge(const PTaskParameter& parameter);

protected:
	void Init();

protected:
	std::map<uint32_t, uint64_t> m_mapRegister;
};
