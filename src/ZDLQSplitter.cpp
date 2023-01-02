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
#include "ZDLQSplitter.h"
#include <QMetaObject>


ZDLQSplitter::ZDLQSplitter(ZDLWidget *parent):ZDLWidget(parent){
	box = new QVBoxLayout(this);
	split = new QSplitter(this);
	//std::cout << "SPLIT: Using ZDLWidget as parent" << std::endl;
	box->addWidget(split);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

ZDLQSplitter::ZDLQSplitter(QWidget *parent):ZDLWidget(parent){
	box = new QVBoxLayout(this);
	split = new QSplitter(this);
	//std::cout << "SPLIT: Using QWidget as parent" << std::endl;
	box->addWidget(split);
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

QSplitter *ZDLQSplitter::getSplit(){
	return split;
}

void ZDLQSplitter::addChild(ZDLWidget *child){
	split->addWidget(child);
	//Hook the child up with our internal signal handling
	child->setZParent(this);
}

void ZDLQSplitter::addChild(QWidget *child){
	split->addWidget(child);
}
