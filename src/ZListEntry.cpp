#include "ZListEntry.hpp"

ZListEntry::ZListEntry(QString originator, QString type){
	this->originator = originator;
	this->type = type;
}

ZListEntry::ZListEntry(ZQWidget originator, QString type){
	/* Default until I change this class */
	this->originator = "net.vectec.zdlsharp.qzdl.zqwidget";
	this->type = type;
}

void ZListEntry::addData(QString key, QVariant newData){
	data.insert(key,newData);
}

void ZListEntry::removeData(QString key){
	data.remove(key);
}

QVariant ZListEntry::getData(QString key){
	return data.value(key);
}


