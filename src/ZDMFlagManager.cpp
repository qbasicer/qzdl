#include "ZDMFlagManager.h"

ZDMFlagManager::ZDMFlagManager(QWidget *parent) :QObject(parent){
}

void ZDMFlagManager::addCheckbox(ZDMFlagCheckbox* box){
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