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

void RegisterFileType(char *ext,char *type,char *nicetype,char *exe,char* command,int icon){
	int i=0;
	char *tmp=0,*tmp2=0;
	i=strlen(command)+strlen(exe)+MAX_PATH;
	memset((tmp=malloc(i*sizeof(char))),0,i*sizeof(char));
	memset((tmp2=malloc(i*sizeof(char))),0,i*sizeof(char));
	// Delete the old extensions
	snprintf(tmp,i,"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\%s",ext);
	SHDeleteKey(HKEY_CURRENT_USER,tmp);
	SHDeleteKey(HKEY_CLASSES_ROOT,ext);
	SHDeleteKey(HKEY_CLASSES_ROOT,type);
	// Add new keys
	RegSetValue(HKEY_CLASSES_ROOT,ext,REG_SZ,type,strlen(type));
	RegSetValue(HKEY_CLASSES_ROOT,type,REG_SZ,nicetype,strlen(nicetype));
	snprintf(tmp,i,"%s\\DefaultIcon",type);
	snprintf(tmp2,i,"%s,%d",_pgmptr,icon);
	RegSetValue(HKEY_CLASSES_ROOT,tmp,REG_SZ,tmp2,strlen(tmp2));
	snprintf(tmp,i,"%s\\shell\\open\\command",type);
	snprintf(tmp2,i,"\"%s\" %s",exe,command);
	RegSetValue(HKEY_CLASSES_ROOT,tmp,REG_SZ,tmp2,strlen(tmp2));
	// Free crap
	free(tmp);free(tmp2);
}


#endif

