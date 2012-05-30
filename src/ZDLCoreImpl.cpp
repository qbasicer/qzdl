#include "ZDLCoreImpl.h"
#include "zdlcommon.h"

ZDLCoreImpl::ZDLCoreImpl(QStringList args){
	mutex = new QMutex(QMutex::Recursive);
	LOGDATAO() << "ZDLCoreImpl START" << endl;
	lastPid = 0;
	this->args = args;
}

ZPID ZDLCoreImpl::loadPluginPath(QString path){
	LOGDATAO() << "Unimplemented function" << endl;
	return BAD_ZPID; 
}

ZPID ZDLCoreImpl::loadPluginName(QString name){
	LOGDATAO() << "Unimplemented function" << endl;
        return BAD_ZPID;
}

bool ZDLCoreImpl::unloadPlugin(ZPID pid){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

ZPID ZDLCoreImpl::findPluginByName(QString name){
	LOGDATAO() << "Unimplemented function" << endl;
        return BAD_ZPID;
}

bool ZDLCoreImpl::findPluginsByRegex(QString regex, QVector<ZPID> &result){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

QVariant ZDLCoreImpl::pluginCall(ZPID pid, QString func, QVector<QVariant> args){
	LOGDATAO() << "Unimplemented function" << endl;
        return QVariant();
}

bool ZDLCoreImpl::addTab(QString tabName, QWidget *widget){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

bool ZDLCoreImpl::removeTab(QWidget *widget){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

QString ZDLCoreImpl::getValue(QString section, QString variable){
	LOGDATAO() << "Unimplemented function" << endl;
        return QString("");
}

bool ZDLCoreImpl::setValue(QString section, QString variable, QString value){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

bool ZDLCoreImpl::hasSection(QString section){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

bool ZDLCoreImpl::hasVariable(QString section, QString variable){
	LOGDATAO() << "Unimplemented function" << endl;
        return false;
}

ZPID ZDLCoreImpl::registerPlugin(ZDLPluginApi *plugin){
	LOGDATAO() << "Unimplemented function" << endl;
	PluginEntry *pe = new PluginEntry();
	pe->pid = ++lastPid;
	pe->plugin = plugin;
	pe->origin = 1;
	pe->runner = new ZDLPluginRunner(this, plugin);
	pe->runner->start();
	return pe->pid;
}

QStringList ZDLCoreImpl::getArgs(){
	return args;
}

void ZDLCoreImpl::fireInternalEvent(int evtid, void* payload){
}

