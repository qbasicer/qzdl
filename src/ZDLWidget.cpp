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
 
#include <iostream>
#include <QtGui>
#include <QApplication>
#include "ZDLWidget.h"
#include <QMetaObject>

/* Our parent is a ZDLWidget, connect our signals up */
ZDLWidget::ZDLWidget(ZDLWidget *parent):QWidget(parent){
	setZParent(parent);
	/* Try to compact the interface */
	setContentsMargins(0,0,0,0);
}


/*
 * This is the guts of the 'magic'.  If a child widget triggers a 'buildChildren(this)' or a 'buildParent(this)',
 * the widgets get triggered along the tree.  Signalling a parent propagates the signal up, and signals all their children
 * back down, eventually signalling all children.  It is required to call both buildChildren and buildParent, buildParent
 * a child will prevent signals from passing through it if they're the originators.  Using buildChildren may be useful when you only want
 * to save the configuration of children. Creating a new ZDLWidget and setting it's parent to be another ZDLWidget will
 * automatically enroll the new widget into the saving process.  There is special logic inside ZDLMainWindow to 'jumper'
 * save events between tabs, since ZDLMainWindow, in itself, is not a ZDLWidget.  When switching away from a tab, ZDLMainWindow
 * forces a save of the tab being left, and a load on the tab being switched to.  This is to ensure that changes in one tab
 * get reflected in another tab, such as adding an IWAD.
 *
 * This system is designed so that the widgets internally hold their own state, and only save their state when an event happens
 * such as an exit condition, launch condition, or tab change.  This is an attempt to improve responsiveness and reduce the
 * number of read operations that must be done.  For example, every time a user moves a file in a list, it shouldn't be required
 * to resave that data into the configuration file.
 *
 * At no point is configuration designed to be written to a file on the fly.  ZDLConfig serves as an in-memory database.
 * While ZDLConfig has been improved for multithreaded access, it should not be generally considered thread safe.
 */
void ZDLWidget::setZParent(ZDLWidget *parent){
	zparent = parent;
	connect(parent, SIGNAL( buildChildren(ZDLWidget*) ), this, SLOT(notifyFromParent(ZDLWidget*)));
	connect(this, SIGNAL(buildParent(ZDLWidget*)), parent, SLOT(notifyFromChild(ZDLWidget*)));
	connect(parent, SIGNAL( readChildren(ZDLWidget*) ), this, SLOT(readFromParent(ZDLWidget*)));
	connect(this, SIGNAL(readParent(ZDLWidget*)), parent, SLOT(readFromChild(ZDLWidget*)));
}

ZDLWidget::ZDLWidget(){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
}

ZDLWidget::ZDLWidget(QWidget *parent):QWidget(parent){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
}

void ZDLWidget::notifyFromChild(ZDLWidget *origin){
	/* If we didn't send this request */
	if (origin != this){
		/* Tell our children */
		emit buildChildren(origin);
		/* And our parent, since we're sending upwards */
		emit buildParent(origin);
		/* And save ourselves */
		rebuild();
	}
}

void ZDLWidget::notifyFromParent(ZDLWidget *origin){
	if (origin != this){
		/* Our parent told us to save, build our children */ 
		emit buildChildren(origin);
		/* And save ourselves */
		rebuild();
	}
}

void ZDLWidget::readFromChild(ZDLWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		emit readParent(origin);
		newConfig();
	}
}

void ZDLWidget::readFromParent(ZDLWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		newConfig();
	}
}

void ZDLWidget::rebuild(){
}

void ZDLWidget::newConfig(){
}






