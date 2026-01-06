#include "module.h"
#include "taskmanager.h"
#include "command.h"
#include <QSettings>

Module::Module(QObject *parent /*= nullptr*/)
	: QObject(parent)
{
	Init();
}

Module::~Module()
{
	auto manager = TaskManager_SpecifyThread::GetInstance();
	manager->Cancel(TT_Query_UserName);
	manager->Cancel(TT_Query_UserSex);
	manager->Cancel(TT_Query_UserAge);
}

void Module::cmdQuertName(const PTaskParameter& parameter)
{
	if (!parameter->m_data)
		return;
	std::shared_ptr<StringData> data = std::dynamic_pointer_cast<StringData>(parameter->m_data);
	if (!data)
		return;

	QSettings settings(QString::fromStdString(data->m_str), QSettings::IniFormat);
	QString strValue = settings.value("User/Name").toString();

	PTaskParameter reply = TaskParameter::Create(TT_Reply_UserName);
	reply->m_informer = this;
	std::shared_ptr<StringData> dataReply = std::make_shared<StringData>();
	dataReply->m_str = strValue.toStdString();
	reply->m_data = dataReply;
	TaskManager_UI::GetInstance()->Notify(reply);
}

void Module::cmdQuertSex(const PTaskParameter& parameter)
{
	if (!parameter->m_data)
		return;
	std::shared_ptr<StringData> data = std::dynamic_pointer_cast<StringData>(parameter->m_data);
	if (!data)
		return;

	QSettings settings(QString::fromStdString(data->m_str), QSettings::IniFormat);
	QString strValue = settings.value("User/Sex").toString();

	PTaskParameter reply = TaskParameter::Create(TT_Reply_UserSex);
	reply->m_informer = this;
	std::shared_ptr<StringData> dataReply = std::make_shared<StringData>();
	dataReply->m_str = strValue.toStdString();
	reply->m_data = dataReply;
	TaskManager_UI::GetInstance()->Notify(reply);
}

void Module::cmdQuertAge(const PTaskParameter& parameter)
{
	if (!parameter->m_data)
		return;
	std::shared_ptr<StringData> data = std::dynamic_pointer_cast<StringData>(parameter->m_data);
	if (!data)
		return;

	QSettings settings(QString::fromStdString(data->m_str), QSettings::IniFormat);
	QString strValue = settings.value("User/Age").toString();

	PTaskParameter reply = TaskParameter::Create(TT_Reply_UserAge);
	reply->m_informer = this;
	std::shared_ptr<StringData> dataReply = std::make_shared<StringData>();
	dataReply->m_str = strValue.toStdString();
	reply->m_data = dataReply;
	TaskManager_UI::GetInstance()->Notify(reply);
}

void Module::Init()
{
	auto manager = TaskManager_SpecifyThread::GetInstance();
	manager->Register<Module>(TT_Query_UserName, this, &Module::cmdQuertName);
	manager->Register<Module>(TT_Query_UserSex, this, &Module::cmdQuertSex);
	manager->Register<Module>(TT_Query_UserAge, this, &Module::cmdQuertAge);
}
