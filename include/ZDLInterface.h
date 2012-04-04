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

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "ZDLMultiPane.h"


class ZDLInterface: public ZQWidget{
Q_OBJECT
public: 
	ZDLInterface( QWidget *parent=0);
	void startRead();
	void writeConfig();
	virtual void newConfig();
	virtual void rebuild();
private slots:
	void sendSignals();
	void mclick();
	void launch();
	void ampclick();
	void saveConfigFile();
	void loadConfigFile();
	void aboutClick();
	void showCommandline();
	void showNewDMFlagger();
	void exitzdl();
	void loadZdlFile();
	void saveZdlFile();
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
};
#endif
