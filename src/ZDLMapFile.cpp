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

const char iwad_m[]={'I', 'W', 'A', 'D'};
const char pwad_m[]={'P', 'W', 'A', 'D'};
const char zip_m[]={'P', 'K', 0x03, 0x04};

ZDLMapFile *ZDLMapFile::getMapFile(QString file){
	QFile fileio(file);
	ZDLMapFile *mapfile=NULL;

	if (fileio.open(QIODevice::ReadOnly)) {
		char magic[4];

		if (fileio.read(magic, 4)==4) {
			if (!strncmp(magic, iwad_m, 4)||!strncmp(magic, pwad_m, 4))
				mapfile=new DoomWad(file);
			else if (!strncmp(magic, zip_m, 4))
				mapfile=new ZLibPK3(file);
		}

		fileio.close();
	}

	return mapfile;
}
