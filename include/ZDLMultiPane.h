/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018  Lcferrum
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

#include <QtGui>
#include <QObject>
#include "ZDLWidget.h"

class PlayersValidator: public QIntValidator {
public:
    PlayersValidator(QObject *parent, QComboBox *cb): QIntValidator(1, INT_MAX, parent), validated_cb(cb) {}
    virtual void fixup(QString &input) const;
private:
	QComboBox *validated_cb;
};

class ZDLMultiPane: public ZDLWidget{
Q_OBJECT
public: 
	ZDLMultiPane( ZDLWidget *parent=0);
	void setLaunchButton(QPushButton *some_btn) { launch_btn=some_btn; }
	void enableAll();
	void disableAll();
	virtual void newConfig();
	virtual void rebuild();
private:
	QIntValidator *max_int_validator;
	PlayersValidator *players_validator;
	QComboBox *gMode;
	QLineEdit *tHostAddy;
	QComboBox *gPlayers;
	QLineEdit *tFragLimit;
	QLineEdit *tTimeLimit;
	QPushButton *launch_btn;
	QLineEdit *bDMFlags;
	QLineEdit *bDMFlags2;
	QComboBox *extratic;
	QComboBox *netmode;
	QLineEdit *portNo;
	QComboBox *dupmode;
	QComboBox *savegame;

protected slots:
	void ModePlayerChanged(int idx);
	void EditPlayers(int idx);
	void EditSave(int idx);
	void VerbosePopup();
	void dmflags();
	void dmflags2();
};

#endif

