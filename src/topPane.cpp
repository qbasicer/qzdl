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
 
#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "topPane.h"
#include "filePane.h"
#include "settingPane.h"
#include "ZQSplitter.h"

topPane::topPane(ZQWidget *parent): ZQWidget(parent){
	QHBoxLayout *box = new QHBoxLayout(this);

	ZQSplitter *split = new ZQSplitter(this);
	QSplitter *rsplit = split->getSplit();
	
	
	filePane *fpane = new filePane(rsplit);
	settingPane *spane = new settingPane(rsplit);
	
	split->addChild(fpane);
	split->addChild(spane);


	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
	box->setSpacing(2);
	box->addWidget(rsplit);
}
