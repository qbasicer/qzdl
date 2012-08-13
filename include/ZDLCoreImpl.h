#ifndef _ZDLCOREIMPL_H_
#define _ZDLCOREIMPL_H_

class ZDLCoreImpl;

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
			runlock = new QMutex();
			hasAlias = false;
			loader = NULL;
		}

		ZPID pid;
		ZDLPluginApi *plugin;
		int origin;
		ZDLPluginRunner *runner;
		QMutex *runlock;
		bool hasAlias;
		QPluginLoader *loader;
};

class ZDLServiceList;

class ZDLCoreImpl : public ZDLCoreApi {
	public:
		ZDLCoreImpl(QStringList args);
                virtual ZPID loadPluginPath(QString path);
                virtual ZPID loadPluginName(QString name);
                virtual bool unloadPlugin(ZPID pid);
                virtual ZPID findPluginByName(QString name);
                virtual bool findPluginsByRegex(QString regex, QVector<ZPID> &result);
                virtual QVariant pluginCall(ZPID pid, QString func, QList<QVariant> args);
                virtual bool addTab(QString tabName, QWidget *widget);
                virtual bool removeTab(QWidget *widget);
                virtual QString getValue(QString section, QString variable);
                virtual bool setValue(QString section, QString variable, QString value);
                virtual bool hasSection(QString section);
                virtual bool hasVariable(QString section, QString variable);
		virtual QStringList getArgs();
		virtual bool waitForProcessExit(ZPID pid);
		virtual bool runFunctionInGui(QString func, QList<QVariant> args, bool async);
		virtual bool registerAlias(QString alias);
		virtual bool deregisterAlias(QString alias);
		virtual bool getAllPlugins(QList<ZPID> &result);
                virtual bool getPluginProperties(ZPID pid, QHash<QString,QVariant> &props);
                virtual ZPID getCurrentZPID();
                virtual bool attachThread(QThread *peer);
                virtual bool detatchThread(QThread *peer);
                virtual unsigned int getSequence();
                virtual bool isGuiThread();
                virtual ZPID getPidForService(QString service);
                virtual bool getAllPidsForServices(QList<ZPID> &list);
                virtual bool registerServiceHandler(QString service);
                virtual bool deregisterServiceHandler(QString service);
                virtual bool getAllServices(QStringList &list);
		virtual bool runService(ZPID pid, QString service, QHash<QString, QVariant> &payload);


		// Internal private API
		virtual ZPID registerPlugin(ZDLPluginApi *plugin);
		virtual void fireInternalEvent(int evtid, void* payload);
		virtual void registerGuiThread();
	protected:
		// Functions
		void lock(){
			mutex->lock();
		}

		void unlock(){
			mutex->unlock();
		}

		PluginEntry *getEntryForCurrentThread();
		virtual bool runFunctionInGuiInternal(ZDLPluginApi* plugin, QString func, QList<QVariant> args, bool async);

		// Members
		QHash<ZPID, PluginEntry*> plugins;
		QHash<QString, ZPID> aliases;
		QHash<QThread*, ZPID> threads;

		QHash<QString, ZDLServiceList*> services;


		QMutex *mutex;
		ZPID lastPid;
		QStringList args;
		QThread *guiThread;
};

class ZDLServiceList {
	public:
		ZDLServiceList(QString serviceName);
		void addHandler(ZPID pid, bool preferred = false);
		void removeHandler(ZPID pid);
		ZPID getPreferred();
		void setPreferred(ZPID pid);
		ZPID getPid();
	private:
		QString serviceName;
		QList<ZPID> serviceHandlers;
		ZPID preferred;
};

#endif

