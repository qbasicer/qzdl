# This file is part of qZDL
# Copyright (C) 2007-2010  Cody Harris
# Copyright (C) 2018-2019  Lcferrum
#
# qZDL is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

TEMPLATE = app
TARGET = qzdl
DESTDIR = ./release
QT += core gui
CONFIG += release static
QMAKE_CFLAGS += -fno-strict-aliasing
QMAKE_CXXFLAGS += -Wno-missing-field-initializers
DEFINES += _ZDL_NO_WARNINGS _ZDL_NO_WFA NOMINMAX STATIC QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2
MOC_DIR += moc
OBJECTS_DIR += release
UI_DIR += uic
RCC_DIR += rcc
INCLUDEPATH += \
    miniz \
    res/XPM \
    include
HEADERS += \
    miniz/miniz.h \
    include/ZDLFileInfo.h \
    include/ZDLConfiguration.h \
    include/ZDLFileListable.h \
    include/ZDLListEntry.hpp \
    include/ZDLMapFile.h \
    include/ZDLNullDevice.h \
    include/libwad.h \
    include/zdlcommon.h \
    include/zdlconf.hpp \
    include/zdlline.hpp \
    include/zdlsection.hpp \
    include/zdlvariables.hpp \
    include/ZLibPK3.h \
    include/ZDLAboutDialog.h \
    include/ZDLConfigurationEvents.h \
    include/ZDLConfigurationManager.h \
    include/ZDLDMFlagCheckbox.h \
    include/ZDLDMFlagManager.h \
    include/ZDLFileList.h \
    include/ZDLFilePane.h \
    include/ZDLIWadList.h \
    include/ZDLImportDialog.h \
    include/ZDLInterface.h \
    include/ZDLListWidget.h \
    include/ZDLListable.h \
    include/ZDLMainWindow.h \
    include/ZDLMultiPane.h \
    include/ZDLNameInput.h \
    include/ZDLNameListable.h \
    include/ZDLQSplitter.h \
    include/ZDLSettingsPane.h \
    include/ZDLSettingsTab.h \
    include/ZDLSourcePortList.h \
    include/ZDLWidget.h \
    include/ZDMFlagDialog.h \
    include/ZDMFlagPicker.h
SOURCES += \
    miniz/miniz.c \
    src/ZDLAboutDialog.cpp \
    src/ZDLConfiguration.cpp \
    src/ZDLConfigurationEvents.cpp \
    src/ZDLConfigurationManager.cpp \
    src/ZDLDMFlagCheckbox.cpp \
    src/ZDLDMFlagManager.cpp \
    src/ZDLFileInfo.cpp \
    src/ZDLFileList.cpp \
    src/ZDLFileListable.cpp \
    src/ZDLFilePane.cpp \
    src/ZDLIWadList.cpp \
    src/ZDLImportDialog.cpp \
    src/ZDLInterface.cpp \
    src/ZDLListEntry.cpp \
    src/ZDLListWidget.cpp \
    src/ZDLListable.cpp \
    src/ZDLMainWindow.cpp \
    src/ZDLMapFile.cpp \
    src/ZDLMultiPane.cpp \
    src/ZDLNameInput.cpp \
    src/ZDLNameListable.cpp \
    src/ZDLQSplitter.cpp \
    src/ZDLSettingsPane.cpp \
    src/ZDLSettingsTab.cpp \
    src/ZDLSourcePortList.cpp \
    src/ZDLWidget.cpp \
    src/ZDMFlagDialog.cpp \
    src/ZDMFlagPicker.cpp \
    src/extras.cpp \
    src/libwad.cpp \
    src/qzdl.cpp \
    src/zdlconf.cpp \
    src/zdlline.cpp \
    src/zdlsection.cpp \
    src/zdlvariables.cpp \
    src/ZLibPK3.cpp
OTHER_FILES += \
    res/XPM/adown.xpm \
    res/XPM/aup.xpm \
    res/XPM/bmp_logo.xpm \
    res/XPM/gph_dna.xpm \
    res/XPM/gph_dnt.xpm \
    res/XPM/gph_dps.xpm \
    res/XPM/gph_fld.xpm \
    res/XPM/gph_mns.xpm \
    res/XPM/gph_pls.xpm \
    res/XPM/gph_upa.xpm \
    res/XPM/gph_upt.xpm \
    res/XPM/ico_icon.xpm
