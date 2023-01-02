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
 
#ifndef _ZINFOBAR_H_
#define _ZINFOBAR_H_

#include <QtWidgets>
#include <QObject>
#include "ZDLWidget.h"

#define INFOBAR_NORMAL	0
#define INFOBAR_WARNING	1
#define INFOBAR_INFO	2

class ZDLInfoBar : public ZDLWidget{
Q_OBJECT
	public: 
		ZDLInfoBar( ZDLWidget *parent=0);
		void setMessage(const char* messgae, int icon);
	protected:
		QLabel *txtlbl;
		QLabel *lbl;
		QColor warning;
		QColor info;
		QColor normal;
	protected slots:
		virtual void hidebar();
		virtual void more();
	signals:
		void moreclicked();
};

#endif

