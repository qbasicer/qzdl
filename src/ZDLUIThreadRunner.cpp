#include "ZDLUIThreadRunner.h"

ZDLUIThreadRunner *uiRunner;

ZDLUIThreadRunner::ZDLUIThreadRunner(){
	uiRunner = this;
	connect(this, SIGNAL(runLinkage(ZDLPluginApi*, QString*, QVariant*, QMutex*)), this, SLOT(runPlugin(ZDLPluginApi*, QString*, QVariant*, QMutex*)),Qt::QueuedConnection);
}

void ZDLUIThreadRunner::runInGui(ZDLPluginApi *plugin, QString func, QList<QVariant> args, QMutex *lock){
	//TODO: Actually allow for async calls.  As it stands, async calls are ignored

	//Create and set up blocing mutex
	QMutex mutex;
	mutex.lock();

	//Create a QVariant container for the QList
	QVariant vari = QVariant(args);

	//Queue up this function to run
	emit runLinkage(plugin, &func, &vari, &mutex);

	//Try locking again.  Will block until the GUI thread has returned
	mutex.lock();

	//If we were requested to lock, unlock, unblocking the owner
	if(lock){
		lock->unlock();
	}
}


void ZDLUIThreadRunner::runPlugin(ZDLPluginApi *plugin, QString *func, QVariant *args, QMutex *lock){
	// Create a local stack copy of the arguments
	QVariant ptrTo = QVariant(*args);
	QString ptrStr = QString(*func);

	// Convert the arguments back to a list
	QList<QVariant> largs = QList<QVariant>(ptrTo.toList());

	// Make the call to the plugin
	plugin->pluginCall(ptrStr, largs);

	// If we were passed a lock, unlock it
	if(lock != NULL){
		lock->unlock();
	}
}

