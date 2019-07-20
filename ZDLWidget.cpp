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
#include <QtWidgets>
#include <QApplication>
#include "ZDLWidget.h"
#include <QMetaObject>


ZDLWidget::ZDLWidget(ZDLWidget *parent):QWidget(parent){
	setZParent(parent);
	//std::cout << "Using ZDLWidget as parent" << std::endl;
	setContentsMargins(0,0,0,0);
}

void ZDLWidget::setZParent(ZDLWidget *parent){
	zparent = parent;
	connect(parent, &ZDLWidget::buildChildren, this, &ZDLWidget::notifyFromParent);
	connect(this, &ZDLWidget::buildParent, parent, &ZDLWidget::notifyFromChild);
	connect(parent, &ZDLWidget::readChildren, this, &ZDLWidget::readFromParent);
	connect(this, &ZDLWidget::readParent, parent, &ZDLWidget::readFromChild);
}

ZDLWidget::ZDLWidget(){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
	//std::cout << "Using QWidget as parent" << std::endl;
}

ZDLWidget::ZDLWidget(QWidget *parent):QWidget(parent){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
	//std::cout << "Using QWidget as parent" << std::endl;
}

void ZDLWidget::notifyFromChild(ZDLWidget *origin){
	if (origin != this){
		emit buildChildren(origin);
		emit buildParent(origin);
		//fromDownstream(origin);
		rebuild();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from child (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZDLWidget::notifyFromParent(ZDLWidget *origin){
	if (origin != this){
		emit buildChildren(origin);
		//fromUpstream(origin);
		rebuild();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from parent (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZDLWidget::readFromChild(ZDLWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		emit readParent(origin);
		newConfig();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from child (of new config) (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZDLWidget::readFromParent(ZDLWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		newConfig();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from parent (of new config) (I am " << qmo->className() << ")" << std::endl;
	}
}

//void ZDLWidget::fromDownstream(ZDLWidget *origin){
//}

//void ZDLWidget::fromUpstream(ZDLWidget *origin){
//}

void ZDLWidget::rebuild(){
}

void ZDLWidget::newConfig(){
}






