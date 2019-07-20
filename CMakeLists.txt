cmake_minimum_required(VERSION 3.0)
project(qzdl LANGUAGES C CXX)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

qt_wrap_cpp(
	zdl
	ZDL_MOC_OUTFILES
	${PROJECT_SOURCE_DIR}/ZDLAboutDialog.h
	${PROJECT_SOURCE_DIR}/ZDLAdvancedMultiplayerDialog.h
	${PROJECT_SOURCE_DIR}/ZDLDMFlagCheckbox.h
	${PROJECT_SOURCE_DIR}/ZDLDMFlagManager.h
	${PROJECT_SOURCE_DIR}/ZDLDMFlagManager.h
	${PROJECT_SOURCE_DIR}/ZDLFilePane.h
	${PROJECT_SOURCE_DIR}/ZDLInfoBar.h
	${PROJECT_SOURCE_DIR}/ZDLInterface.h
	${PROJECT_SOURCE_DIR}/ZDLListWidget.h
	${PROJECT_SOURCE_DIR}/ZDLListable.h
	${PROJECT_SOURCE_DIR}/ZDLMainWindow.h
	${PROJECT_SOURCE_DIR}/ZDLMultiPane.h
	${PROJECT_SOURCE_DIR}/ZDLNameInput.h
	${PROJECT_SOURCE_DIR}/ZDLNameListable.h
	${PROJECT_SOURCE_DIR}/ZDLQSplitter.h
	${PROJECT_SOURCE_DIR}/ZDLSettingsPane.h
	${PROJECT_SOURCE_DIR}/ZDLSettingsTab.h
	${PROJECT_SOURCE_DIR}/ZDLWidget.h
	${PROJECT_SOURCE_DIR}/ZDMFlagPicker.h
)

add_executable(
	zdl
	ZDLAboutDialog.cpp
	ZDLAdvancedMultiplayerDialog.cpp
	ZDLDMFlagCheckbox.cpp
	ZDLDMFlagManager.cpp
	ZDLFileList.cpp
	ZDLFileListable.cpp
	ZDLFilePane.cpp
	ZDLIWadList.cpp
	ZDLInfoBar.cpp
	ZDLInterface.cpp
	ZDLListWidget.cpp
	ZDLListable.cpp
	ZDLMainWindow.cpp
	ZDLMapFile.cpp
	ZDLMultiPane.cpp
	ZDLNameInput.cpp
	ZDLNameListable.cpp
	ZDLQSplitter.cpp
	ZDLSettingsPane.cpp
	ZDLSettingsTab.cpp
	ZDLSourcePortList.cpp
	ZDLWidget.cpp
	ZDMFlagPicker.cpp
	libwad.cpp
	qzdl.cpp
	${ZDL_MOC_OUTFILES}
	${PROJECT_SOURCE_DIR}/zdlconf/zdlconf.cpp
	${PROJECT_SOURCE_DIR}/zdlconf/inih-r44/ini.c
)

target_include_directories(zdl PRIVATE ${PROJECT_SOURCE_DIR})
target_include_directories(zdl PRIVATE ${PROJECT_SOURCE_DIR}/zdlconf)
target_include_directories(zdl PRIVATE ${PROJECT_SOURCE_DIR}/zdlconf/inih-r44)
target_link_libraries(zdl Qt5::Core Qt5::Widgets)
target_compile_definitions(zdl PRIVATE FALSE=0 TRUE=1)
