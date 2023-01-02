#ifndef ZDLSETTINGSTAB_H
#define ZDLSETTINGSTAB_H

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
 
#include <QtWidgets>
#include <QObject>
#include "ZDLWidget.h"
#include "ZDLSourcePortList.h"
#include "ZDLIWadList.h"

class ZDLSettingsTab: public ZDLWidget{
Q_OBJECT
public: 
	ZDLSettingsTab(QWidget *parent);
	virtual void rebuild();
	virtual void newConfig();
	void startRead();
	void writeConfig();

	// For the unit tests
	QString getAlwaysArgs();
	Qt::CheckState getLaunchClose();
	Qt::CheckState getShowPaths();
	Qt::CheckState getLaunchZDL();
	Qt::CheckState getSavePaths();
protected slots:
	void reloadConfig();
	void pathToggled(bool enabled);
private:
	QLineEdit *alwaysArgs;
	QCheckBox *launchClose;
	ZDLIWadList *iwadList;
	ZDLSourcePortList *sourceList;
	QCheckBox *showPaths;
	QCheckBox *launchZDL;
	QCheckBox *savePaths;
};

#endif
