#include "ZDLConfiguration.h"

#include <iostream>

using namespace std;

ZDLConfiguration::ZDLConfiguration(){
	//TODO These need to migrate
	QSettings system(QSettings::IniFormat,QSettings::SystemScope,"Vectec Software","qZDL",NULL);
	QSettings user(QSettings::IniFormat,QSettings::UserScope,"Vectec Software","qZDL",NULL);
	paths[CONF_SYSTEM] = system.fileName();
	paths[CONF_USER] = user.fileName();
	paths[CONF_FILE] = "zdl.ini";
	confs[CONF_SYSTEM] = new ZDLConf();
	confs[CONF_USER] = new ZDLConf();
	confs[CONF_FILE] = new ZDLConf();
}

QString ZDLConfiguration::getPath(ConfScope scope){
	if(scope >= NUM_CONFS || scope < 0){
		return QString();
	}
	return paths[scope];
}

ZDLConf* ZDLConfiguration::getConf(ConfScope scope){
	if(scope >= NUM_CONFS || scope < 0){
		return NULL;
	}
	return confs[scope];
}

QString ZDLConfiguration::getString(QString section, QString key, int *ok,ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	if(ok){*ok = FALSE;}
	return QString();
}

int ZDLConfiguration::getInt(QString section, QString key, int *ok, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	if(ok){*ok = FALSE;}
	return 0;
}

bool ZDLConfiguration::setString(QString section, QString key, QString value, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(value);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}

bool ZDLConfiguration::setInt(QString section, QString key, int value, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(value);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}

bool ZDLConfiguration::hasVariable(QString section, QString key, ConfScope scope, ScopeRules rules){
	Q_UNUSED(section);
	Q_UNUSED(key);
	Q_UNUSED(scope);
	Q_UNUSED(rules);
	return FALSE;
}
