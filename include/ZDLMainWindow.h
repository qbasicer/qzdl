/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 
#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "ZQWidget.h"
#include "zdlInterface.h"
#include "zSettingsPane.h"
#include "ZUpdater.h"

class ZDLMainWindow: public QMainWindow{
    Q_OBJECT
public:
	ZDLMainWindow( QWidget *parent=0);
	~ZDLMainWindow();
	void startRead();
	void writeConfig();
	void setUpdater(ZUpdater *zup);
	QStringList getArguments();
	QString getExecutable();
public slots:
	void launch();
	void quit();
	void tabChange(int index);
	void manageUpdate();
	void badLaunch();
	void newUpdate();

protected:
	zdlInterface* intr;
	zSettingsPane* settings;
	ZUpdater* zup;
	
	int procerr;
	QAction *qact2;
};
#endif
