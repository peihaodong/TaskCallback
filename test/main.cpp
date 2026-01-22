#include <QtWidgets/QApplication>
#include "application.h"
#include "mainwindow.h"
#include "module.h"
#include <memory>
#include "taskmanager.h"

/***********************************************
   > Author: 裴浩东
   > Date: 2026.01.06
   > GitHub: https://github.com/peihaodong/TaskCallback.git
   > Remark: 如果对你有帮助，请帮我在GitHub上点个Star
**********************************************/

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
