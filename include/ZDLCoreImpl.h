#ifndef _ZDLCOREIMPL_H_
#define _ZDLCOREIMPL_H_

#include <QtCore>
#include "ZDLApiCommon.h"
#include "ZDLCoreApi.h"

class ZDLCoreImpl : public ZDLCoreApi {
	public:
                virtual ZPID loadPluginPath(QString path);
                virtual ZPID loadPluginName(QString name);
                virtual bool unloadPlugin(ZPID pid);
                virtual ZPID findPluginByName(QString name);
                virtual bool findPluginsByRegex(QString regex, QVector<ZPID> &result);
                virtual QVariant pluginCall(ZPID pid, QString func, QVector<QVariant> args);
                virtual bool addTab(QString tabName, QWidget *widget);
                virtual bool removeTab(QWidget *widget);
                virtual QString getValue(QString section, QString variable);
                virtual bool setValue(QString section, QString variable, QString value);
                virtual bool hasSection(QString section);
                virtual bool hasVariable(QString section, QString variable);
};

#endif

