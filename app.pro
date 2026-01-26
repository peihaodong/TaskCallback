# 指定项目的类型或模板（app:应用程序 lib:库 subdirs:多子目录项目 plugin:插件）
TEMPLATE = subdirs

# 指定编译器或构建系统在构建项目时将遵循某种特定的顺序
CONFIG += ordered

# 指定一个包含子目录的项目
SUBDIRS += \
		task_callback \
		test
		
# 声明依赖关系
test.depends = task_callback

# 指定启动项目
test.CONFIG += run_first