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


class ZQWidget: public QWidget{
Q_OBJECT
public: 
	ZQWidget(ZQWidget *parent);
	ZQWidget(QWidget *parent);
	ZQWidget();
	virtual void setZParent(ZQWidget *parent);
	virtual void rebuild();
	virtual void newConfig();
signals:
	void buildChildren(ZQWidget*);
	void buildParent(ZQWidget*);
	void readChildren(ZQWidget*);
	void readParent(ZQWidget*);
public slots:
	virtual void notifyFromChild(ZQWidget*);
	virtual void notifyFromParent(ZQWidget*);
	virtual void readFromChild(ZQWidget*);
	virtual void readFromParent(ZQWidget*);
//protected:
//	virtual void fromUpstream(ZQWidget *origin);
//	virtual void fromDownstream(ZQWidget *origin);

private:
	ZQWidget *zparent;
};
#endif

