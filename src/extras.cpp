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
	LOGDATA() << "Type:" << type << ", niceType:" << niceType << " exec:" << exec << " command:" << command << " iconIndex:" << iconIndex << endl;

	QString toDelete("Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\");
	toDelete += "ext";

	// Remove old data
	SHDeleteKey(HKEY_CURRENT_USER,toDelete.toStdWString().c_str());
	SHDeleteKey(HKEY_CLASSES_ROOT,extension.toStdWString().c_str());
	SHDeleteKey(HKEY_CLASSES_ROOT,type.toStdWString().c_str());
	LOGDATA() << "Deleting HKEY_CURRENT_USER\\" << toDelete << endl;
	LOGDATA() << "Deleting HKEY_CLASSES_ROOT\\" << extension << endl;
	LOGDATA() << "Deleting HKEY_CLASSES_ROOT\\" << type << endl;

	// Add new keys
	RegSetValue(HKEY_CLASSES_ROOT,extension.toStdWString().c_str(),REG_SZ,type.toStdWString().c_str(),type.length());
	RegSetValue(HKEY_CLASSES_ROOT,type.toStdWString().c_str(),REG_SZ,niceType.toStdWString().c_str(),niceType.length());

	LOGDATA() << "Setting HKEY_CLASSES_ROOT\\" << extension << " to " << type << endl;
	LOGDATA() << "Setting HKEY_CLASSES_ROOT\\" << type << " to " << niceType << endl;

	// Set up ICON
	QString regIconPath(type);
	regIconPath += "\\DefaultIcon";
	QString iconLocation(exec);
	iconLocation += "," + QString::number(iconIndex);
	RegSetValue(HKEY_CLASSES_ROOT,regIconPath.toStdWString().c_str(),REG_SZ,iconLocation.toStdWString().c_str(),iconLocation.length());

	LOGDATA() << "Setting HKEY_CLASSES_ROOT\\" << regIconPath << " to " << iconLocation << endl;

	// Set up command
	QString regCmdPath(type);
	regCmdPath += "\\shell\\open\\command";
	QString cmd("\"");
	cmd += exec + "\" " + command;
	RegSetValue(HKEY_CLASSES_ROOT,regCmdPath.toStdWString().c_str(),REG_SZ,cmd.toStdWString().c_str(), cmd.length());
	LOGDATA() << "Setting HKEY_CLASSES_ROOT\\" << regCmdPath << " to " << cmd << endl;

	//Done
}

#endif

/* Lifted straight from ZDL 3.0 */
 //////////////////////////
// From DoomDefs.h (by Bio)
typedef struct{
	char type[4];	// Either "IWAD" or "PWAD"
	int lumps;	// Number of lumps in the dir
	int dir;		// the offset of the start of the dir
}WADHEAD;
typedef struct{
	int start;	// where the lump starts
	int length;	// how long the lump is
	char name[8];	// the lump's name (e.g. "MAP01")
}LUMPHEAD;

int stricmp(const char* a, const char* b){
	QString qa(a);
	QString qb(b);
	return qa.compare(qb,Qt::CaseInsensitive);
}


