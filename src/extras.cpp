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
 
#include <iostream>
#include <list>
#include <string>
#include <vector>
using namespace std;
#include <zdlcommon.h>

QString chomp(QString in)
{
	QString ret = in.trimmed();
	return ret;
}

int countLines(ZDLConf *cnf)
{
	int count = 0;
	for(int i = 0; i < cnf->sections.size(); i++){
		ZDLSection *sc = cnf->sections[i];
		count += sc->lines.size();
	}	
	return count;
} 

#if defined(Q_WS_WIN)

#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <shlwapi.h>

void RegisterFileTypeQt(QString extension, QString type, QString niceType, QString exec, QString command, int iconIndex);
void RegisterFileType(char *ext,char *type,char *nicetype,char *exe,char* command,int icon){
	RegisterFileTypeQt(QString(ext), QString(type), QString(nicetype), QString(exe), QString(command), icon);
}

void RegisterFileTypeQt(QString extension, QString type, QString niceType, QString exec, QString command, int iconIndex){
	LOGDATA() << "Registering extension " << extension << endl;
	QString toDelete("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\").append("ext");

	// Remove old data
	SHDeleteKey(HKEY_CURRENT_USER,toDelete.toStdWString().c_str());
	SHDeleteKey(HKEY_CLASSES_ROOT,extension.toStdWString().c_str());
	SHDeleteKey(HKEY_CLASSES_ROOT,type.toStdWString().c_str());

	// Add new keys
	RegSetValue(HKEY_CLASSES_ROOT,extension.toStdWString().c_str(),REG_SZ,type.toStdWString().c_str(),type.length());
	RegSetValue(HKEY_CLASSES_ROOT,type.toStdWString().c_str(),REG_SZ,nicetype.toStdWString().c_str(),nicetype.length());

	// Set up ICON
	QString regIconPath(type).append("\\DefaultIcon");
	QString iconLocation("").append(exec).append(QString::number(icon);
	RegSetValue(HKEY_CLASSES_ROOT,regIconPath.toStdWString().c_str(),REG_SZ,iconLocation.toStdWString().c_str(),iconLocation.length()));

	// Set up command
	QString regCmdPath(type).append("\\shell\\open\\command");
	QString command("\"");
	command += exe + "\" " + command;
	RegSetValue(HKEY_CLASSES_ROOT,regCmdPath.toStdWString().c_str(),REG_SZ,command.toStdWString().c_str(), command.length());

	//Done
}


#endif

