#ifndef _ZDLUITHREADRUNNER_H_
#define _ZDLUITHREADRUNNER_H_

#include "ZDLPluginApi.h"

class ZDLUIThreadRunner : public QObject {
	Q_OBJECT
	public:
		ZDLUIThreadRunner();
		void runInGui(ZDLPluginApi *plugin, QString func, QList<QVariant> args, QMutex *lock);
	signals:
		void runLinkage(ZDLPluginApi *plugin, QString *func, QVariant *args, QMutex *lock);
	protected slots:
		void runPlugin(ZDLPluginApi *plugin, QString *func, QVariant *args, QMutex *lock);
};

#endif

