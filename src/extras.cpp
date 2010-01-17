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

char* chomp(string in)
{
	while (in.length() > 0){
		if (in[0] == ' ' || in[0] == '\n' || in[0] == '\r' || in[0] == '\t'){
			in.erase(0);
		}else{
			break;
		}
	}
	while (in.length() > 0){
		if (in[in.length()-1] == ' ' || in[in.length()-1] == '\n' || in[in.length()-1] == '\r' || in[in.length()-1] == '\t'){
			in.erase(in.length()-1);
		}else{
			break;
		}
	}
	return (char*)in.c_str();
}

int countLines(ZDLConf *cnf)
{
	int count = 0;
	list<ZDLSection*>::iterator itr;
	for (itr = (cnf->sections).begin(); itr != (cnf->sections).end();itr++){
		list <ZDLLine*>::iterator itr2;
		ZDLSection *sc = (*itr);
		for (itr2 = (sc->lines).begin(); itr2 != (sc->lines).end();itr2++){
			count++;
		}
	}
	return count;

} 
