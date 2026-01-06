# 指定该项目所需的Qt模块
QT    += core gui

# 如果QT版本大于4，则将widgets模块加入到项目中
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# 指定编译后生成的可执行文件或库文件的名称
TARGET = test

# 指定项目的类型或模板（app:应用程序 lib:库 subdirs:多子目录项目 plugin:插件）
TEMPLATE = app

# 指定编译后生成的可执行文件或库文件输出的目标目录
DESTDIR = $$PWD/../bin

# 指定预处理器宏定义
DEFINES += QT_DEPRECATED_WARNINGS

# 启用c++11
# 强制编译器生成调试信息
CONFIG += \
		c++11 \
		force_debug_info

# 指定头文件
HEADERS += \
		mainwindow.h \
		module.h \
		command.h \
		application.h

# 指定源文件
SOURCES += \
		main.cpp \
		mainwindow.cpp \
		module.cpp \
		command.cpp \
		application.cpp
		
# 指定ui文件
FORMS += \
		mainwindow.ui

# 指定附加包含目录（$$PWD代表pro文件所在的目录）
INCLUDEPATH += \
		$$PWD/../task_callback/include

# 指定附加库目录
LIBS += \
		-L$$PWD/../lib

# 指定附加依赖项
LIBS += \
		-ltask_callback