/*
 * This file is part of qZDL
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

#include "ZLibPK3.h"
#include "miniz.h"

ZLibPK3::ZLibPK3(const QString &file):
	map_names(), file(file)
{}

QStringList ZLibPK3::getMapNames()
{
	return map_names;
}

bool ZLibPK3::open()
{
	mz_zip_archive zip_archive={};
	map_names.clear();

	if (mz_zip_reader_init_file(&zip_archive, qPrintable(file), 0)) {
		if (mz_uint fnum=mz_zip_reader_get_num_files(&zip_archive)) {
			mz_zip_archive_file_stat file_stat;

			for (mz_uint i=0; i<fnum; i++) {
				if (mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
					QFileInfo zname(file_stat.m_filename);
					if (!zname.path().compare("maps", Qt::CaseInsensitive)&&zname.baseName().length())
						map_names<<zname.baseName().left(8).toUpper();
				} else {
					break;
				}
			}
		}

		mz_zip_reader_end(&zip_archive);
	}

	return map_names.length();
}

bool ZLibPK3::isCompressed()
{
	return true;
}
