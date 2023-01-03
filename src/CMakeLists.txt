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

# Retrieve the absolute path to qmake and then use that path to find
# the binaries
get_target_property(_qmake_executable Qt5::qmake IMPORTED_LOCATION)
get_filename_component(_qt_bin_dir "${_qmake_executable}" DIRECTORY)
find_program(MACDEPLOYQT_EXECUTABLE macdeployqt HINTS "${_qt_bin_dir}")

if (WIN32)
    find_program(WINDEPLOYQT_EXECUTABLE windeployqt HINTS "${_qt_bin_dir}")
    add_custom_command(TARGET zdl POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E
            env PATH="${_qt_bin_dir}" "${WINDEPLOYQT_EXECUTABLE}"
				--dir deploy
                "$<TARGET_FILE:zdl>"
        COMMENT "Running windeployqt..."
    )

    install(
        DIRECTORY "${CMAKE_BINARY_DIR}/deploy/"
        TYPE BIN
    )

	set(CPACK_GENERATOR "NSIS")
	include(CPack)	
endif(WIN32)

if (APPLE)
    add_custom_command(TARGET zdl POST_BUILD
        COMMAND "${MACDEPLOYQT_EXECUTABLE}"
            "$<TARGET_FILE_DIR:zdl>/../.."
            -always-overwrite
        COMMENT "Running macdeployqt...
(not codesigning, afterwards run 'codesign --deep -f -s - $<TARGET_FILE_DIR:zdl>/../..')"
    )

	set(CPACK_GENERATOR "DragNDrop")
	include(CPack)
endif(APPLE)
