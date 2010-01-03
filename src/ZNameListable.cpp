
#include "ZListable.h"
#include "ZNameListable.h"
#include <string>
#include <QFileInfo>
#include <iostream>

using namespace std;

ZNameListable::ZNameListable( QListWidget *parent, int type, const char* file, const char* name):ZListable(parent, type){
	QString list = QString("%1 [%2]").arg(name).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;
	displayName = name;
}


ZNameListable::ZNameListable( QListWidget *parent, int type, QString &file, QString &name):ZListable(parent, type){
	QString list = QString("%1 [%2]").arg(name).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;
	displayName = name;

}

const char* ZNameListable::getFile(){
	return fileName.toStdString().c_str();
}

const char* ZNameListable::getName(){
	return displayName.toStdString().c_str();
}
	


