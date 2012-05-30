#ifndef _ZDLCOREIMPL_H_
#define _ZDLCOREIMPL_H_

#include <QtCore>
#include "ZDLApiCommon.h"
#include "ZDLCoreApi.h"
#include "ZDLPluginApi.h"
#include "ZDLPluginRunner.h"

class PluginEntry {
	public:
		PluginEntry(){
			pid = 0;
			plugin = NULL;
			origin = 0;
		}

		ZPID pid;
		ZDLPluginApi *plugin;
		int origin;
		ZDLPluginRunner *runner;
};

class ZDLCoreImpl : public ZDLCoreApi {
	public:
		ZDLCoreImpl(QStringList args);
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
		virtual QStringList getArgs();

		// Internal private API
		virtual ZPID registerPlugin(ZDLPluginApi *plugin);
		virtual void fireInternalEvent(int evtid, void* payload);
	protected:
		// Functions
		void lock(){
			mutex->lock();
		}

		void unlock(){
			mutex->unlock();
		}

		// Members
		QHash<ZPID, PluginEntry*> plugins;
		QMutex *mutex;
		ZPID lastPid;
		QStringList args;
};

#endif

