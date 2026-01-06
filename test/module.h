#pragma once

#include <QObject>
#include "taskcommon.h"

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
