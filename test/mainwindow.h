#pragma once

#include <QMainWindow>
#include "ui_mainwindow.h"
#include "taskcommon.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow();

protected:
	void Init();

protected slots:
	void slotNameClicked();
	void slotSexClicked();
	void slotAgeClicked();

	void cmdReplyLabel(const PTaskParameter& parameter);

private:
	Ui::MainWindowClass *ui;

	QString m_strIniPath;
};
