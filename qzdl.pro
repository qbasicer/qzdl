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
TARGET = zdl
DESTDIR = release
MOC_DIR = moc
UI_DIR = uic
RCC_DIR = rcc

QT += core gui
CONFIG += release
DEFINES += _ZDL_NO_WARNINGS _ZDL_NO_WFA NOMINMAX QT_HAVE_MMX QT_HAVE_3DNOW QT_HAVE_SSE QT_HAVE_MMXEXT QT_HAVE_SSE2

*g++|*g++-64|*g++-32 {
    QMAKE_CFLAGS += -fno-strict-aliasing
    QMAKE_CXXFLAGS += -Wno-missing-field-initializers -Wno-maybe-uninitialized
}

# For static build supply CONFIG+=static as additional argument in qmake build step for selected kit
static {
    DEFINES += STATIC

    *g++|*g++-64|*g++-32 {
        QMAKE_LFLAGS += -static-libstdc++ -static-libgcc -Wl,--as-needed
    }
}

INCLUDEPATH += \
    miniz \
    include \
    res/XPM
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
    include/ZLibPK3.h \
    include/ZDLAboutDialog.h \
    include/ZDLConfigurationManager.h \
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
    include/ZDLWidget.h
SOURCES += \
    miniz/miniz.c \
    src/ZDLAboutDialog.cpp \
    src/ZDLConfiguration.cpp \
    src/ZDLConfigurationManager.cpp \
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
    src/libwad.cpp \
    src/qzdl.cpp \
    src/zdlconf.cpp \
    src/zdlline.cpp \
    src/zdlsection.cpp \
    src/ZLibPK3.cpp
OTHER_FILES += \
    res/XPM/adown.xpm \
    res/XPM/aup.xpm \
    res/XPM/bmp_logo.xpm \
    res/XPM/gph_dna.xpm \
    res/XPM/gph_dnt.xpm \
    res/XPM/gph_ast.xpm \
    res/XPM/gph_fld.xpm \
    res/XPM/gph_mns.xpm \
    res/XPM/gph_pls.xpm \
    res/XPM/gph_upa.xpm \
    res/XPM/gph_upt.xpm \
    res/XPM/ico_icon.xpm
