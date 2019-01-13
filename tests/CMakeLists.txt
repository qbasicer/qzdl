cmake_minimum_required(VERSION 3.10)

cmake_policy(SET CMP0071 NEW)

project(test_zdl LANGUAGES CXX)

find_package(Qt5Test REQUIRED)
find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
enable_testing()

qt_wrap_cpp(
	test_zdl
	TEST_ZDL_MOC_OUTFILES
	../include/ZDLConfigurationEvents.h
	../include/ZDLInfoBar.h
	../include/ZDLWidget.h
	../include/ZDLMainWindow.h
	../include/ZDLInterface.h
	../include/ZDLFilePane.h
	../include/ZDLMultiPane.h
	../include/ZDLQSplitter.h
	../include/ZDLListWidget.h
	../include/ZDMFlagPicker.h
	../include/ZDLSettingsTab.h
	../include/ZDLNameInput.h
	../include/ZDLAboutDialog.h
	../include/ZDLFileListable.h
	../include/ZDLListable.h
	../include/ZDLNameListable.h
	../include/ZDLSettingsPane.h
	../include/ZDLAdvancedMultiplayerDialog.h
)

add_executable(
	${PROJECT_NAME}
	tst_qzdl.cpp
	../src/libwad.cpp
	../src/ZDLConfigurationManager.cpp
	../src/ZDLConfigurationEvents.cpp
	../src/ZDLInfoBar.cpp
	../src/ZDLWidget.cpp
	../src/ZDLConfiguration.cpp
	../src/zdlconf.cpp
	../src/extras.cpp
	../src/zdlsection.cpp
	../src/zdlline.cpp
	../src/zdlsection.cpp
	../src/ZDLMainWindow.cpp
	../src/ZDLInterface.cpp
	../src/ZDLFilePane.cpp
	../src/ZDLFileList.cpp
	../src/ZDLMultiPane.cpp
	../src/ZDLQSplitter.cpp
	../src/ZDLListWidget.cpp
	../src/ZDMFlagPicker.cpp
	../src/ZDLSettingsTab.cpp
	../src/ZDLIWADList.cpp
	../src/ZDLNameInput.cpp
	../src/ZDLAboutDialog.cpp
	../src/ZDLFileListable.cpp
	../src/ZDLListable.cpp
	../src/ZDLNameListable.cpp
	../src/ZDLSettingsPane.cpp
	../src/ZDLSourcePortList.cpp
	../src/ZDLMapFile.cpp
	../src/ZDLAdvancedMultiplayerDialog.cpp
	${TEST_ZDL_MOC_OUTFILES}
)
add_test(${PROJECT_NAME} COMMAND ${PROJECT_NAME})

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Test Qt5::Core Qt5::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ../include)

file(COPY resources/dummy.wad DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)
file(COPY resources/qZDL.ini DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/resources)

# Seems to be needed for ZDLConfiguration.cpp
target_compile_definitions(test_zdl PRIVATE FALSE=0 TRUE=1)

