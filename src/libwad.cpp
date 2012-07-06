#include <QtCore>
#include "libwad.h"
#include "zdlcommon.h"

struct waddir {
	int lumpstart;
	int lumpsize;
	char lumpname[8];
};

DoomWad::DoomWad(QString file){
	QFile *pfile = new QFile(file);
	dev = pfile;
	LOGDATAO() << "Opening wad from our own QFile" << endl;
	shouldClose = true;
}

DoomWad::DoomWad(QIODevice *dev){
	LOGDATAO() << "Opening wad from QIODevice" << endl;
	this->dev = dev;
	shouldClose = false;
}

DoomWad::~DoomWad(){
	if(shouldClose){
		this->dev->close();
		delete this->dev;
		this->dev = NULL;
		LOGDATAO() << "Closing device" << endl;
	}
	LOGDATAO() << "Deleteing DoomWad" << endl;
}

bool DoomWad::open(){
	LOGDATAO() << "Opening wad file" << endl;
	if(!dev->isOpen()){
		bool rc = dev->open(QIODevice::ReadOnly);
		if(!rc){
			LOGDATAO() << "Open failed" << endl;
			return rc;
		}
	}
	LOGDATAO() << "Wad file opened, reading data..." << endl;
	bool rc = dev->seek(4);
	if(!rc){LOGDATAO() << "Seek failed" << endl;return rc;}
	char fourbytes[4];
	int rd = dev->read(fourbytes, 4);
	if(rd != 4){LOGDATAO() << "Read failed" << endl; return false;}
	int lumps = *((int*)fourbytes);
	LOGDATAO() << "Lumps: " << lumps << endl;
	rd = dev->read(fourbytes, 4);
	if(rd != 4){LOGDATAO() << "Read failed" << endl; return false;}
	int diroffset = *((int*)fourbytes);
	rc = dev->seek(diroffset);
	if(!rc){LOGDATAO() << "Seek failed" << endl;return rc;}
	QString last("");
	for(int i = 0; i < lumps; i++){
		struct waddir dir;
		rd = dev->read((char*)&dir, sizeof(struct waddir));
		if(rd != sizeof(struct waddir)){
			LOGDATAO() << "Dropping bad lump" << endl;
			continue;
		}
		char tname[9];
		memset(tname, 0, 9);
		memcpy(tname, dir.lumpname, 8);
		QString lumpName = tname;
		int lumpStart = dir.lumpstart;
		int lumpSize = dir.lumpsize;
		WadLump *lump = new WadLump(lumpStart, lumpSize, lumpName, this);
		wadLumps.append(lump);
		if(lumpName == "THINGS" && !last.isEmpty()){
			LOGDATAO() << "New level" << endl;
			levelnames << last;
		}else{
			LOGDATAO() << "Not a level: " << last << endl;
		}
		last = lumpName;
		
	}
	return true;
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
	return levelnames;
}

bool DoomWad::isCompressed(){
	return false;
}

/******************** WADLUMP ***********************/

WadLump::WadLump(int start, int size, QString name, DoomWad *par){
	lumpStart = start;
	lumpSize = size;
	lumpName = name;
	parent = par;
	LOGDATAO() << "New lump " << name << endl;
}

QString WadLump::getName(){
	return QString(lumpName);
}

int WadLump::getSize(){
	return lumpSize;
}



