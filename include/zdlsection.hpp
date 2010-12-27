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

#include <vector>

class ZDLSection{
	friend class ZDLVariables;
public:
	ZDLSection(const char *name);
	~ZDLSection();
	int addLine(const char *linedata);
	char *getName();
	void setSpecial(int inFlags);
	char *findVariable(const char* variable);
	int hasVariable(const char* variable);
	void deleteVariable(const char* variable);
	int setValue(const char *variable, const char *value);
	int streamWrite(std::ostream &stream);
	int getRegex(const char* regex, std::vector<ZDLLine*> &vctr);
	std::list<ZDLLine*> lines;
private:
	int reads;
	int writes;
	ZDLLine *findLine(const char *inVar);
	int flags;
	std::string sectionName;
};
