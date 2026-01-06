#pragma once

#include <QApplication>
#include "taskcommon.h"

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
