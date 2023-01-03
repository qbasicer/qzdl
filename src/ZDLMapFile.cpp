#include "zdlcommon.h"
#include "ZDLMapFile.h"

#include "libwad.h"

ZDLMapFile *ZDLMapFile::getMapFile(QString file){
	QFileInfo fileInfo(file);
	if(!fileInfo.exists()){
		return NULL;
	}
	QFile fileio(file);
	if(!fileio.open(QIODevice::ReadOnly)){
		return NULL;
	}
	QByteArray bytes = fileio.read(4);
	fileio.close();
	if(bytes.size() != 4){
		return NULL;
	}
	if(bytes[0] == 'P' && bytes[1] == 'W' && bytes[2] == 'A' && bytes[3] == 'D'){
		// PWAD file
		DoomWad *wad = new DoomWad(file);
		return wad;
	}else if(bytes[0] == 'I' && bytes[1] == 'W' && bytes[2] == 'A' && bytes[3] == 'D'){
		// IWAD file
		DoomWad *wad = new DoomWad(file);
		return wad;
	}
	// Unsupported format
	return NULL;
}
