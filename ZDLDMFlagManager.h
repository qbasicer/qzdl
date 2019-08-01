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
 
#ifndef _ZDMFLAGMANAGER_H_
#define _ZDMFLAGMANAGER_H_

#include <QtCore>
#include <QObject>
#include "ZDLWidget.h"
#include "ZDLDMFlagCheckbox.h"
#include <QVector>

class ZDLDMFlagManager: public QObject{
Q_OBJECT
	public: 
		ZDLDMFlagManager(QWidget *parent=0);
		void addCheckbox(ZDLDMFlagCheckbox* box);
		int getValue();
		void setValue(int value);
		void forceRecalc();
	signals:
		void valueChanged(int);
	public slots:
		void stateChanged(int);
	protected:
		QVector<ZDLDMFlagCheckbox*> checks;
};

#endif