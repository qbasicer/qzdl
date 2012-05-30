#include "ZDLUIThreadRunner.h"

ZDLUIThreadRunner *uiRunner;

ZDLUIThreadRunner::ZDLUIThreadRunner(){
	uiRunner = this;
	connect(this, SIGNAL(runLinkage(ZDLPluginApi*, QString, QVariant, QMutex*)), this, SLOT(runPlugin(ZDLPluginApi*, QString, QVariant, QMutex*)),Qt::QueuedConnection);
}

void ZDLUIThreadRunner::runInGui(ZDLPluginApi *plugin, QString func, QVector<QVariant> args, QMutex *lock){
	QList<QVariant> list = args.toList();
	emit runLinkage(plugin, func, list, lock);
}


void ZDLUIThreadRunner::runPlugin(ZDLPluginApi *plugin, QString func, QVariant args, QMutex *lock){
	qDebug() << "Running GUI impl";
	//pluginCall(QString func, QVector<QVariant> args)
	QList<QVariant> list = args.toList();
        QVector<QVariant> vargs = QVector<QVariant>::fromList(list);

	plugin->pluginCall(func, vargs);
	if(lock != NULL){
		lock->unlock();
	}
	qDebug() << "Finished GUI impl";
}

