#include <QtWidgets/QApplication>
#include "application.h"
#include "mainwindow.h"
#include "module.h"
#include <memory>
#include "taskmanager.h"

int main(int argc, char *argv[])
{
	Application a(argc, argv);

	TaskManager_UI::GetInstance()->SetUINotify([&](PTaskParameter parameter) {
		a.DoUITask(parameter);
	});

	std::shared_ptr<Module> m = std::make_shared<Module>();

	MainWindow w;
	w.show();

    return a.exec();
}
