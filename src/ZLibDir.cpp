/*
 * This file is part of qZDL
 * Copyright (C) 2019  Lcferrum
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

#include <QRegExp>
#include "ZLibDir.h"

ZLibDir::ZLibDir(const QString &file):
	file(file)
{}

ZLibDir::~ZLibDir()
{}

QStringList ZLibDir::getMapNames()
{
	QDir zdir(file);
	QStringList map_names;

	foreach (const QFileInfo &zname, zdir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot)) {
		if (ZDLMapFile *mapfile=ZDLMapFile::getMapFile(zname.filePath())) {
			map_names+=mapfile->getMapNames();
			delete mapfile;
		}
	}

	if (zdir.cd("maps")) {	//CD is case insensitive
		foreach (const QFileInfo &zname, zdir.entryInfoList(QDir::Files|QDir::NoDotAndDotDot)) {
			map_names<<zname.baseName().left(8).toUpper();
		}
	}

	return map_names;
}

QString ZLibDir::getIwadinfoName()
{
	QDir zdir(file);
	QString iwad_name;
	QStringList iwadinfo_filter;
	iwadinfo_filter<<"iwadinfo"<<"iwadinfo.*"; //QDir::Filter is case insensitive by default
	QFileInfoList iwadinfo_list=zdir.entryInfoList(iwadinfo_filter, QDir::Files|QDir::NoDotAndDotDot);

	if (iwadinfo_list.length()) {
		QFile iwadinfo_file(iwadinfo_list.first().filePath());

		if (iwadinfo_file.open(QIODevice::ReadOnly)) {
			QRegExp name_re("\\s+Name\\s*=\\s*\"(.+)\"\\s+", Qt::CaseInsensitive);
			name_re.setMinimal(true);

			if (name_re.indexIn(iwadinfo_file.readAll())>-1)
				iwad_name=name_re.cap(1);

			iwadinfo_file.close();
		}
	}

	return iwad_name;
}

bool ZLibDir::isMAPXX()
{
	QDir zdir(file);
	bool is_mapxx=false;

	if (zdir.cd("maps")) {	//CD is case insensitive
		foreach (const QString &zname, zdir.entryList(QDir::Files|QDir::NoDotAndDotDot)) {
			if (!zname.compare("map01.wad", Qt::CaseInsensitive)||!zname.compare("map01.map", Qt::CaseInsensitive)) {
				is_mapxx=true;
				break;
			}
		}
	}

	return is_mapxx;
}
