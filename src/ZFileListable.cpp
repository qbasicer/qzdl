/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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
 

#include "ZDLListable.h"
#include "ZFileListable.h"
#include <string>
#include <QFileInfo>

using namespace std;

ZFileListable::ZFileListable( QListWidget *parent, int type, const char* file):ZDLListable(parent, type){

	QFileInfo qfile(file);
	QString cname = qfile.fileName();
	QString list = QString("%1 [%2]").arg(cname).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;

}

ZFileListable::ZFileListable( QListWidget *parent, int type, QString file):ZDLListable(parent, type){
	QFileInfo qfile(file);
	QString cname = qfile.fileName();
	QString list = QString("%1 [%2]").arg(cname).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;
}

const char* ZFileListable::getFile(){
	return fileName.toStdString().c_str();
}
	


