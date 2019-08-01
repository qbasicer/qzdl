#include <QtCore>
#include "libwad.h"

struct waddir {
	int lumpstart;
	int lumpsize;
	char lumpname[8];
};

DoomWad::DoomWad(QString file){
	QFile *pfile = new QFile(file);
	dev = pfile;
	// Opening wad from our own QFile
	shouldClose = true;
}

DoomWad::DoomWad(QIODevice *dev){
	// Opening wad from QIODevice
	this->dev = dev;
	shouldClose = false;
}

DoomWad::~DoomWad(){
	if(shouldClose){
		this->dev->close();
		delete this->dev;
		this->dev = NULL;
		// Closing device
	}
	// Deleting DoomWad
}

bool DoomWad::open(){
	// Opening wad file
	if(!dev->isOpen()){
		bool rc = dev->open(QIODevice::ReadOnly);
		if(!rc){
			// Open failed
			return rc;
		}
	}
	// Wad file opened, reading data...
	bool rc = dev->seek(4);
	if (!rc)
	{
		// Seek failed
		return rc;
	}
	char fourbytes[4];
	int rd = dev->read(fourbytes, 4);
	if (rd != 4)
	{
		// Read failed
		return false;
	}
	int lumps = *((int*)fourbytes);
	rd = dev->read(fourbytes, 4);
	if (rd != 4)
	{
		// Read failed
		return false;
	}
	int diroffset = *((int*)fourbytes);
	rc = dev->seek(diroffset);
	if (!rc)
	{
		// Seek failed
		return rc;
	}
	QString last("");
	for(int i = 0; i < lumps; i++){
		struct waddir dir;
		rd = dev->read((char*)&dir, sizeof(struct waddir));
		if(rd != sizeof(struct waddir)){
			// Dropping bad lump
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
			// New level
			levelnames << last;
		}else{
			// Not a level: last
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
	// New lump: name
}

QString WadLump::getName(){
	return QString(lumpName);
}

int WadLump::getSize(){
	return lumpSize;
}



