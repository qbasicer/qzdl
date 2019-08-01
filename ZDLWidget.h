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
 
#include <QMetaObject>
#ifndef _ZQWIDGET_H_
#define _ZQWIDGET_H_
#include <QObject>
#include <QWidget>


class ZDLWidget: public QWidget{
Q_OBJECT
public: 
	ZDLWidget(ZDLWidget *parent);
	ZDLWidget(QWidget *parent);
	ZDLWidget();
	virtual void setZParent(ZDLWidget *parent);
	virtual void rebuild();
	virtual void newConfig();
signals:
	void buildChildren(ZDLWidget*);
	void buildParent(ZDLWidget*);
	void readChildren(ZDLWidget*);
	void readParent(ZDLWidget*);
public slots:
	virtual void notifyFromChild(ZDLWidget*);
	virtual void notifyFromParent(ZDLWidget*);
	virtual void readFromChild(ZDLWidget*);
	virtual void readFromParent(ZDLWidget*);
//protected:
//	virtual void fromUpstream(ZDLWidget *origin);
//	virtual void fromDownstream(ZDLWidget *origin);

private:
	ZDLWidget *zparent;
};
#endif

