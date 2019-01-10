/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018  Lcferrum
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

#include "zdlcommon.h"
#include "ZDLMapFile.h"

#include "libwad.h"
#include "ZLibPK3.h"

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
		LOGDATA() << "PWAD file" << endl;
		DoomWad *wad = new DoomWad(file);
		return wad;
	}else if(bytes[0] == 'I' && bytes[1] == 'W' && bytes[2] == 'A' && bytes[3] == 'D'){
		LOGDATA() << "IWAD file" << endl;
		DoomWad *wad = new DoomWad(file);
		return wad;
	}else if(bytes[0] == 'P' && bytes[1] == 'K' && bytes[2] == (char)0x03 && bytes[3] == (char)0x04){
		LOGDATA() << "ZIP file" << endl;
		return new ZLibPK3(file);
#if defined(SEVENZIP)
	}else if(bytes[0] == '7' && bytes[1] == 'z' && bytes[2] == (char)0xBC && bytes[3] == (char)0xAF){
		LOGDATA() << "7z file";
#endif
	}
	LOGDATA() << "Unsupported format" << endl;
	LOGDATA() << bytes.toHex() << endl;
	return NULL;
}
