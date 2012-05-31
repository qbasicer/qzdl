#ifndef _ZDLCOREAPI_H_
#define _ZDLCOREAPI_H_

class ZDLCoreApi;

#include <QtCore>
#include "ZDLApiCommon.h"
#include "ZDLPluginApi.h"

class ZDLCoreApi {
	public:
		virtual ZPID loadPluginPath(QString path) = 0;
		virtual ZPID loadPluginName(QString name) = 0;
		virtual bool unloadPlugin(ZPID pid) = 0;
		virtual ZPID findPluginByName(QString name) = 0;
		virtual bool findPluginsByRegex(QString regex, QVector<ZPID> &result) = 0;
		virtual QVariant pluginCall(ZPID pid, QString func, QVector<QVariant> args) = 0;
		virtual bool addTab(QString tabName, QWidget *widget) = 0;
		virtual bool removeTab(QWidget *widget) = 0;
		virtual QString getValue(QString section, QString variable) = 0;
		virtual bool setValue(QString section, QString variable, QString value) = 0;
		virtual bool hasSection(QString section) = 0;
		virtual bool hasVariable(QString section, QString variable) = 0;
		virtual QStringList getArgs() = 0;
		virtual bool waitForProcessExit(ZPID pid) = 0;
		virtual bool runFunctionInGui(ZDLPluginApi* plugin, QString func, QVector<QVariant> args, bool async) = 0;
		virtual bool registerAlias(ZDLPluginApi* plugin, QString alias) = 0;
};



#endif
