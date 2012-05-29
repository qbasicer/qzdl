#include "ZDLCoreImpl.h"

ZPID ZDLCoreImpl::loadPluginPath(QString path){
}

ZPID ZDLCoreImpl::loadPluginName(QString name){
}

bool ZDLCoreImpl::unloadPlugin(ZPID pid){
}

ZPID ZDLCoreImpl::findPluginByName(QString name){
}

bool ZDLCoreImpl::findPluginsByRegex(QString regex, QVector<ZPID> &result){
}

QVariant ZDLCoreImpl::pluginCall(ZPID pid, QString func, QVector<QVariant> args){
}

bool ZDLCoreImpl::addTab(QString tabName, QWidget *widget){
}

bool ZDLCoreImpl::removeTab(QWidget *widget){
}

QString ZDLCoreImpl::getValue(QString section, QString variable){
}

bool ZDLCoreImpl::setValue(QString section, QString variable, QString value){
}

bool ZDLCoreImpl::hasSection(QString section){
}

bool ZDLCoreImpl::hasVariable(QString section, QString variable){
}

