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
 
#include "ZDMFlagManager.h"

ZDMFlagManager::ZDMFlagManager(QWidget *parent) :QObject(parent){
}

void ZDMFlagManager::addCheckbox(ZDLDMFlagCheckbox* box){
	connect(box,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));
	checks.append(box);
}

void ZDMFlagManager::stateChanged(int value){
	int val = getValue();
	value = 0;
	emit valueChanged(val);
}

void ZDMFlagManager::forceRecalc(){
	int val = getValue();
	emit valueChanged(val);
}

int ZDMFlagManager::getValue(){
	int val = 0;
	for(int i = 0; i < checks.size(); i++){
		val = val | checks[i]->getValue();
	}
	return val;
}

void ZDMFlagManager::setValue(int value){
	for(int i = 0; i < checks.size(); i++){
		checks[i]->setValue(value);
	}
}