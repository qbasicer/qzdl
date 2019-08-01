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
 
#include "ZDLDMFlagManager.h"

ZDLDMFlagManager::ZDLDMFlagManager(QWidget *parent) :QObject(parent){
}

void ZDLDMFlagManager::addCheckbox(ZDLDMFlagCheckbox* box){
	connect(box, &ZDLDMFlagCheckbox::stateChanged, this, &ZDLDMFlagManager::stateChanged);
	checks.append(box);
}

void ZDLDMFlagManager::stateChanged(int value){
	Q_UNUSED(value);
	int val = getValue();
	emit valueChanged(val);
}

void ZDLDMFlagManager::forceRecalc(){
	int val = getValue();
	emit valueChanged(val);
}

int ZDLDMFlagManager::getValue(){
	int val = 0;
	for(int i = 0; i < checks.size(); i++){
		val = val | checks[i]->getValue();
	}
	return val;
}

void ZDLDMFlagManager::setValue(int value){
	for(int i = 0; i < checks.size(); i++){
		checks[i]->setValue(value);
	}
}
