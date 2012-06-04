#include "ZDLCoreImpl.h"
#include "zdlcommon.h"
#include "ZDLUIThreadRunner.h"

extern ZDLUIThreadRunner *uiRunner; 

ZDLCoreImpl::ZDLCoreImpl(QStringList args){
	mutex = new QMutex(QMutex::Recursive);
	LOGDATAO() << "ZDLCoreImpl START" << endl;
	lastPid = 0;
	this->args = args;
	new ZDLUIThreadRunner();
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
	lock();
	QHashIterator<ZPID, PluginEntry*> i(plugins);
	PluginEntry *tar = NULL;
	while(i.hasNext()){
		i.next();
		PluginEntry *pe = i.value();
		if(pe == NULL){
			continue;
		}
		ZDLPluginApi *plugin = pe->plugin;
		if(plugin == NULL){
			continue;
		}
		QString pname = plugin->getPluginFQDN();
		if(pname == name){
			ZPID id = i.key();
			unlock();
			return id;
		}
	}
	if(aliases.contains(name)){
		ZPID pid = aliases.value(name);
		unlock();
		return pid;
	}
	unlock();
	return BAD_ZPID;
}

bool ZDLCoreImpl::findPluginsByRegex(QString regex, QVector<ZPID> &result){
	LOGDATAO() << "Unimplemented function" << endl;
	return false;
}

QVariant ZDLCoreImpl::pluginCall(ZPID pid, QString func, QList<QVariant> args){
	lock();
	PluginEntry *pe = plugins.value(pid);
	if(pe == NULL){
		unlock();
		return QVariant();
	}
	ZDLPluginApi *plugin = pe->plugin;
	unlock();
	return plugin->pluginCall(func, args);
}

bool ZDLCoreImpl::addTab(QString tabName, QWidget *widget){
	ZPID ui = findPluginByName("net.vectec.zdl.ui");
	if(ui == BAD_ZPID){
		qDebug() << "*** Failed to find UI PID";
		return false;
	}
	QList<QVariant> args;
	args.append(QString(tabName));
	args.append(qVariantFromValue(widget));
	lock();
	PluginEntry *pe = plugins.value(ui);
	if(pe == NULL){
		unlock();
		qDebug() << "*** Failed to find UI object";
		return false;
	}
	ZDLPluginApi *plugin = pe->plugin;
	unlock();
	runFunctionInGui(plugin, QString("addTab"), QList<QVariant>(args), false);
	return true;
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
	//Lock the core
	lock();
	PluginEntry *pe = new PluginEntry();
	pe->pid = ++lastPid;
	pe->plugin = plugin;
	pe->origin = 1;
	pe->runner = new ZDLPluginRunner(this, plugin);
	plugins.insert(pe->pid, pe);
	unlock();
	pe->runner->start();
	return pe->pid;
}

QStringList ZDLCoreImpl::getArgs(){
	return args;
}

void ZDLCoreImpl::fireInternalEvent(int evtid, void* payload){
	switch(evtid){
		case CORE_EVENT_PLG_STOP:
			lock();
			{
				ZDLPluginApi *api = (ZDLPluginApi*)payload;
				QHashIterator<ZPID, PluginEntry*> i(plugins);
				PluginEntry *tar = NULL;
				while(i.hasNext()){
					i.next();
					PluginEntry *pe = i.value();
					if(pe){
						if(pe->plugin == api){
							tar = pe;
							break;
						}
					}
				}
				if(tar != NULL){
					ZPID pid = tar->pid;
					tar = plugins.take(pid);
					//Deregister any aliases this plugin has
					if(tar->hasAlias){
						QHashIterator<QString, ZPID> i(aliases);
						while(i.hasNext()){
							i.next();
							if(i.value() == tar->pid){
								aliases.remove(i.key());
							}
						}
					}
					delete tar->runlock;
					delete tar;
					delete api;
				}
			}
			unlock();
			break;
		default:
			LOGDATAO() << "UNKNOWN EVENT FIRED" << endl;
			break;
	}
}

bool ZDLCoreImpl::waitForProcessExit(ZPID pid){
	while(1){
		lock();
		bool isAlive = plugins.contains(pid);
		unlock();
		if(!isAlive){
			break;
		}
		usleep(250);
	}
	return true;
}

bool ZDLCoreImpl::runFunctionInGui(ZDLPluginApi *plugin, QString func, QList<QVariant> args, bool async){
	QMutex *mutex = NULL;
	if(!async){
		mutex = new QMutex();
		mutex->lock();
	}else{
		printf("ERROR: ASYNC CALLS ARE NOT IMPLEMENTED\n");
	}
	uiRunner->runInGui(plugin, func, args, mutex);
	if(!async){
		mutex->lock();
	}	
}

bool ZDLCoreImpl::registerAlias(ZDLPluginApi* plugin, QString alias){
	lock();
	QHashIterator<ZPID, PluginEntry*> i(plugins);
	PluginEntry *tar = NULL;
	//Resolve plugin pointer to ZPID
	while(i.hasNext()){
		i.next();
		PluginEntry *pe = i.value();
		if(pe){
			if(pe->plugin == plugin){
				tar = pe;
				break;
			}
		}
	}
	//If we have a match
	if(tar == NULL){
		unlock();
		return false;
	}
	// Officially add the alias
	aliases.insert(alias, tar->pid);

	// Flag this entry as having an alias
	tar->hasAlias = true;
	unlock();
	return true;
}

