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
 
#ifndef _MULTIPANE_H_
#define _MULTIPANE_H_

#include <QtWidgets>
#include <QObject>
#include "ZDLWidget.h"

class ZDLMultiPane: public ZDLWidget{
Q_OBJECT
public: 
	ZDLMultiPane( ZDLWidget *parent=0);
	virtual void newConfig();
	virtual void rebuild();

	// For the unit tests
	QString getMode();
	QString getHostAddy();
	QString getPlayers();
	QString getFragLmit();
	QString getDMFlags();
	QString getDMFlags2();
private:
	QComboBox *gMode;
	QLineEdit *tHostAddy;
	QComboBox *gPlayers;
	QLineEdit *tFragLimit;
	QLineEdit *bDMFlags;
	QLineEdit *bDMFlags2;
};

#endif

