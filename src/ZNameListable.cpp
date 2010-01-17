
#include "ZListable.h"
#include "ZNameListable.h"
#include <string>
#include <QFileInfo>
#include <iostream>

using namespace std;

ZNameListable::ZNameListable( QListWidget *parent, int type, const char* file, const char* name):ZListable(parent, type){
	fileName = file;
	displayName = name;
	setName(generateName());
}


ZNameListable::ZNameListable( QListWidget *parent, int type, QString &file, QString &name):ZListable(parent, type){
	fileName = file;
	displayName = name;
	setName(generateName());

}

const char* ZNameListable::getFile(){
	return fileName.toStdString().c_str();
}

const char* ZNameListable::getName(){
	return displayName.toStdString().c_str();
}

void ZNameListable::setDisplayName(QString name){
	displayName = name;
	setName(generateName());
}

void ZNameListable::setFile(QString file){
	fileName = file;
	setName(generateName());
}

QString ZNameListable::generateName(){
	QString list = QString("%1 [%2]").arg(displayName).arg(fileName);
}

