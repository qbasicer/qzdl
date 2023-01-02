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
 
#ifndef _ZDLINTERFACE_H_
#define _ZDLINTERFACE_H_

#include <QtWidgets>
#include <QObject>
#include "ZDLInfoBar.h"
#include "ZDLWidget.h"
#include "ZDLMultiPane.h"


class ZDLInterface: public ZDLWidget{
Q_OBJECT
public: 
	ZDLInterface( QWidget *parent=0);
	void startRead();
	void writeConfig();
	ZDLInfoBar *getInfobar();
	void setInfobarMessage(const char*, int icon = 0);
	virtual void newConfig();
	virtual void rebuild();

	// For the unit tests
	QString getExtraArgs() const;
	QString getMode();
	QString getHostAddy();
	QString getPlayers();
	QString getFragLmit();
	QString getDMFlags();
	QString getDMFlags2();
public slots:
	void loadZdlFile();
	void saveZdlFile();
private slots:
	void sendSignals();
	void mclick();
	void launch();
	void ampclick();
	void aboutClick();
	void showCommandline();
	void showNewDMFlagger();
	void exitzdl();
private:
	QLayout *getBottomPane();
	QLayout *getButtonPane();
	QLayout *getTopPane();
	void buttonPaneNewConfig();
	void bottomPaneRebuild();
	void bottomPaneNewConfig();
	
	QPushButton *btnEpr;
	QPushButton *btnZDL;
	QVBoxLayout *box;
	ZDLMultiPane *mpane;
	QLineEdit *extraArgs;
  ZDLInfoBar *zib;
};
#endif
