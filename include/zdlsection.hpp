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
#include <QtCore>

class ZDLSection{
	friend class ZDLVariables;
public:
	ZDLSection(QString name);
	~ZDLSection();
	int addLine(QString data);
	QString getName();
	void setSpecial(int inFlags);
	QString findVariable(QString variable);
	int hasVariable(QString variable);
	void deleteVariable(QString variable);
	int setValue(QString variable, QString value);
	int streamWrite(QIODevice *stream);
	int getRegex(QString regex, QVector<ZDLLine*> &vctr);
	QVector<ZDLLine*> lines;
	ZDLSection *clone();
	void addLine(ZDLLine* line){
		lines.push_back(line);
	}
private:
	int reads;
	int writes;
	ZDLLine *findLine(QString inVar);
	int flags;
	QString sectionName;
};
