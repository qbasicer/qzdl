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
		virtual bool getAllPlugins(QList<ZPID> &result) = 0;
		virtual bool getPluginProperties(ZPID pid, QHash<QString,QVariant> &props) = 0;
		virtual bool findPluginsByRegex(QString regex, QVector<ZPID> &result) = 0;
		virtual QVariant pluginCall(ZPID pid, QString func, QList<QVariant> args) = 0;
		virtual bool addTab(QString tabName, QWidget *widget) = 0;
		virtual bool removeTab(QWidget *widget) = 0;
		virtual QString getValue(QString section, QString variable) = 0;
		virtual bool setValue(QString section, QString variable, QString value) = 0;
		virtual bool hasSection(QString section) = 0;
		virtual bool hasVariable(QString section, QString variable) = 0;
		virtual QStringList getArgs() = 0;
		virtual bool waitForProcessExit(ZPID pid) = 0;
		virtual bool runFunctionInGui(QString func, QList<QVariant> args, bool async) = 0;
		virtual bool registerAlias(QString alias) = 0;
		virtual bool deregisterAlias(QString alias) = 0;

		/* Return the current ZPID of the plugin */
		virtual ZPID getCurrentZPID() = 0;

		/* Attach peer to the current process */
		virtual bool attachThread(QThread *peer) = 0;

		/* Detach peer from the current process, main thread may not be detached */
		virtual bool detatchThread(QThread *peer) = 0;

		/* Get SEQ #, used for caching purposes */
		virtual unsigned int getSequence() = 0;

		/* isGuiThread(), special thread to determine if we have GUI access */
		virtual bool isGuiThread() = 0;

		/* Given a service name, get a service handler for it */
		virtual ZPID getPidForService(QString service) = 0;

		/* For a given service, get all pids that could handle it */
		virtual bool getAllPidsForServices(QList<ZPID> &list) = 0;

		/* For a given service, register plugin as a service handler */
		virtual bool registerServiceHandler(QString service) = 0;

		/* Remove plugin from list of handlers for server */
		virtual bool deregisterServiceHandler(QString service) = 0;

		/* Return the list of all available services */
		virtual bool getAllServices(QStringList &list) = 0;

		/* Ask pid to run the service.  If pid is BAD_ZPID, automatically select an appropriate service pid */
		virtual bool runService(ZPID pid, QString service, QHash<QString, QVariant> payload) = 0;
};



#endif
