#include "application.h"
#include "taskmanager.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

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
