/*
 * This file is part of qZDL
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

#include <QRegExp>
#include "ZLibPK3.h"
#include "miniz.h"

ZLibPK3::ZLibPK3(const QString &file):
	file(file)
{}

ZLibPK3::~ZLibPK3()
{}

QStringList ZLibPK3::getMapNames()
{
	mz_zip_archive zip_archive={};
	QStringList map_names;

	if (mz_zip_reader_init_file(&zip_archive, qPrintable(file), 0)) {
		if (mz_uint fnum=mz_zip_reader_get_num_files(&zip_archive)) {
			mz_zip_archive_file_stat file_stat;

			for (mz_uint i=0; i<fnum; i++) {
				if (!mz_zip_reader_is_file_a_directory(&zip_archive, i)&&mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
					QFileInfo zname(file_stat.m_filename);
					if (!zname.path().compare("maps", Qt::CaseInsensitive))
						map_names<<zname.baseName().left(8).toUpper();
				}
			}
		}

		mz_zip_reader_end(&zip_archive);
	}

	return map_names;
}

QString ZLibPK3::getIwadinfoName()
{
	mz_zip_archive zip_archive={};
	QString iwad_name;

	if (mz_zip_reader_init_file(&zip_archive, qPrintable(file), 0)) {
		if (mz_uint fnum=mz_zip_reader_get_num_files(&zip_archive)) {
			mz_zip_archive_file_stat file_stat;

			for (mz_uint i=0; i<fnum; i++) {
				if (!mz_zip_reader_is_file_a_directory(&zip_archive, i)&&mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
					QFileInfo zname(file_stat.m_filename);
					if (!zname.baseName().compare("iwadinfo", Qt::CaseInsensitive)&&!zname.path().compare(".")) {
						size_t buf_len;
						void *buf;
						if ((buf=mz_zip_reader_extract_to_heap(&zip_archive, i, &buf_len, 0))) {
							QByteArray char_buf=QByteArray::fromRawData((const char*)buf, buf_len);
							
							QRegExp name_re("\\s+Name\\s*=\\s*\"(.+)\"\\s+", Qt::CaseInsensitive);
							name_re.setMinimal(true);
							if (name_re.indexIn(char_buf)>-1)
								iwad_name=name_re.cap(1);

							mz_free(buf);
						}
						break;
					}
				}
			}
		}

		mz_zip_reader_end(&zip_archive);
	}

	return iwad_name;
}

bool ZLibPK3::isMAPXX()
{
	mz_zip_archive zip_archive={};
	bool is_mapxx=false;

	if (mz_zip_reader_init_file(&zip_archive, qPrintable(file), 0)) {
		if (mz_uint fnum=mz_zip_reader_get_num_files(&zip_archive)) {
			mz_zip_archive_file_stat file_stat;

			for (mz_uint i=0; i<fnum; i++) {
				if (!mz_zip_reader_is_file_a_directory(&zip_archive, i)&&mz_zip_reader_file_stat(&zip_archive, i, &file_stat)) {
					QFileInfo zname(file_stat.m_filename);
					if (!zname.path().compare("maps", Qt::CaseInsensitive)&&(!zname.fileName().compare("map01.wad", Qt::CaseInsensitive)||!zname.fileName().compare("map01.map", Qt::CaseInsensitive))) {
						is_mapxx=true;
						break;
					}
				}
			}
		}

		mz_zip_reader_end(&zip_archive);
	}

	return is_mapxx;
}
