#include <QtCore>
#include "libwad.h"

DoomWad::DoomWad(QString file){
}

DoomWad::DoomWad(QIODevice *dev){
}

bool DoomWad::open(){
	return FALSE;
}

int DoomWad::lumps(){
	return 0;
}

WadLump *DoomWad::getLump(int index){
	return NULL;
}

WadLump *DoomWad::getLumpByName(QString name){
	return NULL;
}

bool DoomWad::addLumps(QList<WadLump*> lumps, DoomWad::AddBehaviour behaviour){
	return false;
}

QStringList DoomWad::getLumpNames(){
	return QStringList();
}

QStringList DoomWad::getMapNames(){
	return QStringList();
}

bool DoomWad::isCompressed(){
	return false;
}

/******************** WADLUMP ***********************/

QString WadLump::getName(){
	return QString("");
}

int WadLump::getSize(){
	return 0;
}



