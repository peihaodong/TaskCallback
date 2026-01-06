#include "mainwindow.h"
#include <QSettings>
#include "command.h"
#include "taskmanager.h"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, ui(new Ui::MainWindowClass())
{
	ui->setupUi(this);

	m_strIniPath = QCoreApplication::applicationDirPath() + "/user_info.ini";

	Init();
}

MainWindow::~MainWindow()
{
	delete ui;

	auto manager = TaskManager_UI::GetInstance();
	manager->Cancel(TT_Reply_UserName);
	manager->Cancel(TT_Reply_UserSex);
	manager->Cancel(TT_Reply_UserAge);
}

void MainWindow::Init()
{
	connect(ui->pbName, &QPushButton::clicked, this, &MainWindow::slotNameClicked);
	connect(ui->pbSex, &QPushButton::clicked, this, &MainWindow::slotSexClicked);
	connect(ui->pbAge, &QPushButton::clicked, this, &MainWindow::slotAgeClicked);

	auto manager = TaskManager_UI::GetInstance();
	manager->Register<MainWindow>(TT_Reply_UserName, this, &MainWindow::cmdReplyLabel);
	manager->Register<MainWindow>(TT_Reply_UserSex, this, &MainWindow::cmdReplyLabel);
	manager->Register<MainWindow>(TT_Reply_UserAge, this, &MainWindow::cmdReplyLabel);
}

void MainWindow::slotNameClicked()
{
	PTaskParameter parameter = TaskParameter::Create(TT_Query_UserName);
	parameter->m_informer = this;
	std::shared_ptr<StringData> data = std::make_shared<StringData>();
	data->m_str = m_strIniPath.toStdString();
	parameter->m_data = data;
	TaskManager_SpecifyThread::GetInstance()->Notify(0, parameter);
}

void MainWindow::slotSexClicked()
{
	PTaskParameter parameter = TaskParameter::Create(TT_Query_UserSex);
	parameter->m_informer = this;
	std::shared_ptr<StringData> data = std::make_shared<StringData>();
	data->m_str = m_strIniPath.toStdString();
	parameter->m_data = data;
	TaskManager_SpecifyThread::GetInstance()->Notify(0, parameter);
}

void MainWindow::slotAgeClicked()
{
	PTaskParameter parameter = TaskParameter::Create(TT_Query_UserAge);
	parameter->m_informer = this;
	std::shared_ptr<StringData> data = std::make_shared<StringData>();
	data->m_str = m_strIniPath.toStdString();
	parameter->m_data = data;
	TaskManager_SpecifyThread::GetInstance()->Notify(0, parameter);
}

void MainWindow::cmdReplyLabel(const PTaskParameter& parameter)
{
	if (!parameter->m_data)
		return;
	std::shared_ptr<StringData> data = std::dynamic_pointer_cast<StringData>(parameter->m_data);
	if (!data)
		return;

	QString str = QString::fromStdString(data->m_str);
	ui->label->setText(str);
}


