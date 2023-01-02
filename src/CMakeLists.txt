set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Widgets REQUIRED)

include(FetchContent)
FetchContent_Declare(
	inih
	GIT_REPOSITORY https://github.com/benhoyt/inih.git
	GIT_TAG r44
)
FetchContent_GetProperties(inih)
if (NOT inih_POPULATED)
	FetchContent_Populate(inih)
endif()

set(ICON ${PROJECT_SOURCE_DIR}/res/zdl3.icns)
set_source_files_properties(${ICON} PROPERTIES MACOSX_PACKAGE_LOCATION Resources)
file(COPY ${ICON_PATH} DESTINATION "zdl.app/Contents/Resources")

add_executable(
	zdl
	MACOSX_BUNDLE
	${ICON}
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
	${CMAKE_CURRENT_SOURCE_DIR}/zdlconf/zdlconf.cpp
	${inih_SOURCE_DIR}/ini.c
)

target_include_directories(zdl PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/zdlconf)
target_include_directories(zdl PRIVATE ${inih_SOURCE_DIR})
target_link_libraries(zdl Qt5::Core Qt5::Widgets)

install(TARGETS zdl
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
    )

set(CPACK_GENERATOR "DragNDrop")
include(CPack)
