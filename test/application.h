#pragma once

#include <QApplication>
#include "taskcommon.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

class Application  : public QApplication
{
	Q_OBJECT

public:
#ifdef Q_QDOC
	Application(int &argc, char **argv);
#else
	Application(int &argc, char **argv, int flags = ApplicationFlags);
#endif
	~Application();

	void DoUITask(PTaskParameter parameter);

signals:
	void signalDoUITask(PTaskParameter parameter);

protected slots:
	void slotDoUITask(PTaskParameter parameter);
};
