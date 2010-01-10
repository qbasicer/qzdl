#include "ZDMFlagCheckbox.h"

ZDMFlagCheckbox::ZDMFlagCheckbox(int dmvalue, int highOn, QString text, QWidget *parent):QCheckBox(text,parent){
	logic = highOn;
	value = dmvalue;
	setValue(0);
}

void ZDMFlagCheckbox::setValue(int value){
	int msk = value & this->value;
	if(logic && msk){
		setCheckState(Qt::Checked);
	}else if(!logic && !msk){
		setCheckState(Qt::Checked);
	}else{
		setCheckState(Qt::Unchecked);
	}
}

int ZDMFlagCheckbox::getValue(){
	if(checkState() == Qt::Checked && logic){
		return value;
	}else if(checkState() == Qt::Unchecked && !logic){
		return value;
	}
	return 0;
}