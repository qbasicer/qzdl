/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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
#include "ZLibDir.h"

union magic_t {
	char n[4];
	qint32 x;
};

const magic_t iwad_m={'I', 'W', 'A', 'D'};
const magic_t pwad_m={'P', 'W', 'A', 'D'};
const magic_t zip_m={'P', 'K', 0x03, 0x04};

ZDLMapFile::~ZDLMapFile()
{}

ZDLMapFile *ZDLMapFile::getMapFile(QString file)
{
	ZDLMapFile *mapfile=NULL;
	QFileInfo file_info(file);
	QString ext=file_info.completeSuffix();
	QRegExp ban_exts("lmp|txt|cfg|ini|deh|bex|zdl|zds|dsg|esg", Qt::CaseInsensitive);	//Blacklist obvious non-map files

	if (file_info.isDir()) {
		mapfile=new ZLibDir(file);
	} else if (ext.length()&&!ban_exts.exactMatch(ext)&&file_info.exists()) {	//Only process files with present non-blacklisted extension
		QFile fileio(file);

		if (fileio.open(QIODevice::ReadOnly)) {
			magic_t file_m;

			if (fileio.read(file_m.n, 4)==4) {
				if (file_m.x==iwad_m.x||file_m.x==pwad_m.x)
					mapfile=new DoomWad(file);
				else if (file_m.x==zip_m.x)
					mapfile=new ZLibPK3(file);
			}
		}

		fileio.close();
	}

	return mapfile;
}
