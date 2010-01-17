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
 
class ZDLVariables;

#

class ZDLConf {
	friend class ZDLVariables;
public:
	enum modes{
		ReadOnly  		= 0x01,
		WriteOnly 		= 0x02,
		ReadWrite 		= ReadOnly | WriteOnly,
		FileRead  		= 0x04,
  		FileWrite 		= 0x08,
		FileReadWrite 	= FileRead | FileWrite,
  		Default			= ReadWrite | FileReadWrite
	};
	
	int readINI(const char *file);
	int writeINI(const char *file);
	//char* getValue(const char *section, const char *variable);
	const char *getValue(const char *lsection, const char *variable, int *status);
	int hasValue(const char *section, const char *variable);
	void deleteValue(const char *lsection, const char *variable);
	int setValue(const char *lsection, const char *variable, int value);
	int setValue(const char *lsection, const char *variable, const char *szBuffer);
	int numberOfSections();
	~ZDLConf();
	ZDLConf(int mode = ZDLConf::Default);
	int reopen(int mode);
	list<ZDLSection*> sections;
	int writeStream(ostream &stream);
	ZDLSection *getSection(const char* section);
private:
	int mode;
	int reads;
	int writes;
	void parse(string in, ZDLSection* current);
	ZDLVariables *vars;
};
