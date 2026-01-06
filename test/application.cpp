#include "application.h"
#include "taskmanager.h"

#ifdef Q_QDOC
Application::Application(int &argc, char **argv)
	: QApplication(argc, argv)
{
	qRegisterMetaType<PTaskParameter>("PTaskParameter");
	connect(this, &Application::signalDoUITask, this, &Application::slotDoUITask);
}
#else
Application::Application(int &argc, char **argv, int flags /*= ApplicationFlags*/)
	: QApplication(argc, argv, flags)
{
	qRegisterMetaType<PTaskParameter>("PTaskParameter");
	connect(this, &Application::signalDoUITask, this, &Application::slotDoUITask);
}
#endif

Application::~Application()
{}

void Application::DoUITask(PTaskParameter parameter)
{
	emit signalDoUITask(parameter);
}

void Application::slotDoUITask(PTaskParameter parameter)
{
	TaskManager_UI::GetInstance()->DoUITask(parameter);
}
