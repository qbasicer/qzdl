cmake_minimum_required(VERSION 3.10)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

project(qzdl LANGUAGES C CXX)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

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
	${PROJECT_SOURCE_DIR}/zdlconf/zdlconf.cpp
	${PROJECT_SOURCE_DIR}/zdlconf/inih-r44/ini.c
)

target_include_directories(zdl PRIVATE ${PROJECT_SOURCE_DIR}/zdlconf)
target_include_directories(zdl PRIVATE ${PROJECT_SOURCE_DIR}/zdlconf/inih-r44)
target_link_libraries(zdl Qt5::Core Qt5::Widgets)
