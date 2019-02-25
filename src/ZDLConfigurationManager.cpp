/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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
 
#include <stdlib.h>

#include <iostream>
#include <list>
#include <string>
#include <vector>

using namespace std;

#include "zdlcommon.h"
#include "ZDLConfigurationManager.h"
#include "ico_icon.xpm"

void ZDLConfigurationManager::init(){
	activeConfig = NULL;
	cdir = "";
	conf = new ZDLConfiguration();
	events = new ZDLConfigurationEvents();
}

ZDLConf *ZDLConfigurationManager::activeConfig;
string ZDLConfigurationManager::cdir;
ZDLWidget* ZDLConfigurationManager::interface;
QString ZDLConfigurationManager::filename;
ZDLConfiguration *ZDLConfigurationManager::conf;
ZDLConfigurationManager::WhyConfig ZDLConfigurationManager::why;
QStringList ZDLConfigurationManager::argv;
QString ZDLConfigurationManager::exec;
ZDLConfigurationEvents* ZDLConfigurationManager::events;

ZDLConfigurationEvents* ZDLConfigurationManager::getEvents(){
	return ZDLConfigurationManager::events;
}

void ZDLConfigurationManager::setExec(QString execu){
	ZDLConfigurationManager::exec = execu;
}

QString ZDLConfigurationManager::getExec(){
	return ZDLConfigurationManager::exec;
}

QStringList ZDLConfigurationManager::getArgv(){
	return ZDLConfigurationManager::argv;
}

void ZDLConfigurationManager::setArgv(QStringList args){
	ZDLConfigurationManager::argv = args;
}

void ZDLConfigurationManager::setWhy(ZDLConfigurationManager::WhyConfig conf){
	ZDLConfigurationManager::why = conf;
}

ZDLConfigurationManager::WhyConfig ZDLConfigurationManager::getWhy(){
	return ZDLConfigurationManager::why;
}


void ZDLConfigurationManager::setInterface(ZDLWidget *widget){
	interface  = widget;
}
ZDLWidget* ZDLConfigurationManager::getInterface(){
	return interface;
}

void ZDLConfigurationManager::setActiveConfiguration(ZDLConf *zconf){
	//cout << "Using new configuration" << endl;
	ZDLConfigurationManager::activeConfig = zconf;
	events->signalNewConfiguration(zconf);
}

ZDLConf* ZDLConfigurationManager::getActiveConfiguration(){
	return ZDLConfigurationManager::activeConfig;
}

void ZDLConfigurationManager::setCurrentDirectory(string dir){
	cdir = dir;
}

QPixmap ZDLConfigurationManager::getIcon(){
	return QPixmap(zdlicon);
}

const char* ZDLConfigurationManager::getCurrentDirectory(){
	return cdir.c_str();
}


QString ZDLConfigurationManager::getConfigFileName(){
	return filename;
}

void ZDLConfigurationManager::setConfigFileName(QString name){
	filename = name;
}

ZDLConfiguration* ZDLConfigurationManager::getConfiguration(){
	return conf;
}

QString getWadLastDir(ZDLConf *zconf, bool dwd_first)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();
	
	if (dwd_first) {
		if (QProcessEnvironment::systemEnvironment().contains("DOOMWADDIR"))
			return QProcessEnvironment::systemEnvironment().value("DOOMWADDIR");
		else if (zconf->hasValue("zdl.general", "wadLastDir"))
			return zconf->getValue("zdl.general", "wadLastDir");
		else
			return zconf->getValue("zdl.general", "lastDir");
	} else {
		if (zconf->hasValue("zdl.general", "wadLastDir"))
			return zconf->getValue("zdl.general", "wadLastDir");
		else if (QProcessEnvironment::systemEnvironment().contains("DOOMWADDIR"))
			return QProcessEnvironment::systemEnvironment().value("DOOMWADDIR");
		else
			return zconf->getValue("zdl.general", "lastDir");
	}
}

QString getSrcLastDir(ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();

	if (zconf->hasValue("zdl.general", "srcLastDir"))
		return zconf->getValue("zdl.general", "srcLastDir");
	else
		return zconf->getValue("zdl.general", "lastDir");
}

QString getSaveLastDir(ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();

	if (zconf->hasValue("zdl.general", "saveLastDir"))
		return zconf->getValue("zdl.general", "saveLastDir");
	else
		return zconf->getValue("zdl.general", "lastDir");
}

QString getZdlLastDir(ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();

	if (zconf->hasValue("zdl.general", "zdlLastDir"))
		return zconf->getValue("zdl.general", "zdlLastDir");
	else
		return zconf->getValue("zdl.general", "lastDir");
}

QString getIniLastDir(ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();

	if (zconf->hasValue("zdl.general", "iniLastDir"))
		return zconf->getValue("zdl.general", "iniLastDir");
	else
		return zconf->getValue("zdl.general", "lastDir");
}

QString getLastDir(ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return QString();

	return zconf->getValue("zdl.general", "lastDir");
}

void saveWadLastDir(QString fileName, ZDLConf *zconf, bool is_dir)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	if (is_dir) {
		zconf->setValue("zdl.general", "wadLastDir", fileName);
		zconf->setValue("zdl.general", "lastDir", fileName);
	} else {
		QFileInfo fi(fileName);
		zconf->setValue("zdl.general", "wadLastDir", fi.absolutePath());
		zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
	}
}

void saveSrcLastDir(QString fileName, ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	QFileInfo fi(fileName);
	zconf->setValue("zdl.general", "srcLastDir", fi.absolutePath());
	zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
}

void saveSaveLastDir(QString fileName, ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	QFileInfo fi(fileName);
	zconf->setValue("zdl.general", "saveLastDir", fi.absolutePath());
	zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
}

void saveZdlLastDir(QString fileName, ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	QFileInfo fi(fileName);
	zconf->setValue("zdl.general", "zdlLastDir", fi.absolutePath());
	zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
}

void saveIniLastDir(QString fileName, ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	QFileInfo fi(fileName);
	zconf->setValue("zdl.general", "iniLastDir", fi.absolutePath());
	zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
}

void saveLastDir(QString fileName, ZDLConf *zconf)
{
	if (!zconf) 
		if (!(zconf=ZDLConfigurationManager::getActiveConfiguration()))
			return;

	QFileInfo fi(fileName);
	zconf->setValue("zdl.general", "lastDir", fi.absolutePath());
}

void VerboseComboBox::showPopup()
{
	emit onPopup();
	QComboBox::showPopup();
}

void VerboseComboBox::hidePopup()
{
	emit onHidePopup();
	QComboBox::hidePopup();
}


QValidator::State EvilValidator::validate(QString &input, int &pos) const
{
	Q_UNUSED(pos);
	Q_UNUSED(input);

	return QValidator::Invalid;
}
