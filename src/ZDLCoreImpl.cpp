#include "ZDLCoreImpl.h"
#include "zdlcommon.h"
#include "ZDLUIThreadRunner.h"

extern ZDLUIThreadRunner *uiRunner; 

#define ZDL_ASSERT_EQUALS(expected, actual)	zdl_assert_core_impl_internal((void*)(expected), (void*)(actual), __PRETTY_FUNCTION__, __FILE__, __LINE__)
#define ZDL_ASSERT_NOT_EQUALS(expected, actual)     zdl_assert_core_impl_internal_not((void*)(expected), (void*)(actual), __PRETTY_FUNCTION__, __FILE__, __LINE__)

static void zdl_assert_core_impl_internal(void* p1, void* p2, const char* func, const char* file, int line){
        if(p1 != p2){
                printf("ASSERT FAILED from %s at %s:%d\n", func, file, line);
                printf("Execpted: %p got: %p\n", p1, p2);
		LOGDATA() << "ASSERT FAILED FROM " << QString(func) << " at " << QString(file) << ":" << line << endl;
        }
}

static void zdl_assert_core_impl_internal_not(void* p1, void* p2, const char* func, const char* file, int line){
	if(p1 == p2){
		printf("ASSERT FAILED from %s at %s:%d\n", func, file, line);
		printf("Execpted: %p not to equal %p\n", p1, p2);
		LOGDATA() << "ASSERT FAILED FROM " << QString(func) << " at " << QString(file) << ":" << line << endl;
	}
}

//Taken from http://lists.trolltech.com/qt-interest/2007-01/thread00120-0.html
class Helper: public QThread {
	public:
		static void msleep(int ms)
		{
			QThread::msleep(ms);
		}
};

ZDLCoreImpl::ZDLCoreImpl(QStringList args){
	mutex = new QMutex(QMutex::Recursive);
	LOGDATAO() << "ZDLCoreImpl START" << endl;
	lastPid = 0;
	guiThread = NULL;
	this->args = args;
	new ZDLUIThreadRunner();
}


bool ZDLCoreImpl::getAllPlugins(QList<ZPID> &result){
	lock();
	QHashIterator<ZPID, PluginEntry*> i(plugins);
	while(i.hasNext()){
		i.next();
		result.append(i.key());
	}
	unlock();
	return true;
}

bool ZDLCoreImpl::getPluginProperties(ZPID pid, QHash<QString,QVariant> &props){
	Q_UNUSED(pid);
	Q_UNUSED(props);
	return false;
}

ZPID ZDLCoreImpl::loadPluginPath(QString path){
	LOGDATAO() << "Loading plugin from " << path << endl;
	QPluginLoader *bootstrapPlugin = new QPluginLoader(path);
	if (bootstrapPlugin->load()){
		LOGDATAO() << "Plugin loaded" << endl;
		QObject *obj = bootstrapPlugin->instance();
		ZDLPluginApi *plg = qobject_cast<ZDLPluginApi *>(obj);
		if(plg == NULL){
			LOGDATAO() << "Failed to cast the plugin" << endl;
			return BAD_ZPID;
		}
		LOGDATAO() << "Handing plugin over to registrar" << endl;
		return registerPlugin(plg);
	}
	LOGDATAO() << "Failed to load() the plugin:" << bootstrapPlugin->errorString() <<  endl;
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
	QRegExp rx(regex);
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
		if(rx.exactMatch(pname)){
			result.append(pe->pid);
		}
	}
	QHashIterator<QString, ZPID> j(aliases);
	while(j.hasNext()){
		j.next();
		if(rx.exactMatch(j.key())){
			ZPID pid = j.value();
			if(!result.contains(pid)){
				result.append(pid);
			}
		}
	}
	unlock();
	return (result.size() > 0);
}

QVariant ZDLCoreImpl::pluginCall(ZPID pid, QString func, QList<QVariant> args){
	// Save the current calling context
	QThread *currentThread = QThread::currentThread();
	ZPID previous = BAD_ZPID;
	lock();
	if(currentThread == NULL){
		LOGDATAO() << "WARNING: Expected a thread, had none" << endl;
		printf("WARNING: Expected a thread, had none\n");
	}else{
		if(threads.contains(currentThread)){
			previous = threads.value(currentThread);
		}
	}

	PluginEntry *pe = plugins.value(pid);
	if(pe == NULL){
		unlock();
		return QVariant();
	}
	ZDLPluginApi *plugin = pe->plugin;
	// Set the current calling context to the destination
	threads[currentThread] = pe->pid;
	unlock();
	QVariant calling = plugin->pluginCall(func, args);
	lock();
	if(previous == BAD_ZPID){
		// We didn't call from anywhere, strip the current thread
		threads.remove(currentThread);
	}else{
		// We called from another plugin, restore their context
		threads[currentThread] = previous;
	}
	unlock();
	return calling;
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
	runFunctionInGuiInternal(plugin, QString("addTab"), QList<QVariant>(args), false);
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

PluginEntry *ZDLCoreImpl::getEntryForCurrentThread(){
	PluginEntry *pe = NULL;
	lock();
	QThread *currentThread = QThread::currentThread();
	if(currentThread){
		if(threads.contains(currentThread)){
			ZPID pid = threads.value(currentThread);
			pe = plugins.value(pid);
		}
	}
	unlock();
	return pe;
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
					QHashIterator<QThread*, ZPID> i(threads);
					while(i.hasNext()){
						i.next();
						if(i.value() == pid){
							threads.remove(i.key());
						}
					}
					delete tar->runlock;
					delete tar;
					delete api;
				}
			}
			unlock();
			break;
		case CORE_EVENT_PLG_START:
			lock();
			{
				QThread *current = QThread::currentThread();
				if(current){
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
						threads.insert(current, pid);
					}

				}
			}
			unlock();
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
		Helper::msleep(250);
	}
	return true;
}

bool ZDLCoreImpl::runFunctionInGui(QString func, QList<QVariant> args, bool async){
        PluginEntry *callee = getEntryForCurrentThread();
	ZDL_ASSERT_NOT_EQUALS(0, callee);
	if(!isGuiThread()){
		return runFunctionInGuiInternal(callee->plugin, func, args, async);
	}else{
		pluginCall(callee->pid, func, args);
		return true;
	}
}

bool ZDLCoreImpl::runFunctionInGuiInternal(ZDLPluginApi *plugin, QString func, QList<QVariant> args, bool async){
	if(isGuiThread()){
		// If we're already in the gui thread, jump straight to running
		plugin->pluginCall(func, args);
		return true;
	}
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
	return true;
}

bool ZDLCoreImpl::registerAlias(QString alias){
	PluginEntry *callee = getEntryForCurrentThread();
	ZDL_ASSERT_NOT_EQUALS(0, callee);
	ZDLPluginApi *plugin = callee->plugin;
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

bool ZDLCoreImpl::deregisterAlias(QString alias){
	return false;
}

ZPID ZDLCoreImpl::getCurrentZPID(){
	lock();
	PluginEntry *callee = getEntryForCurrentThread();
	if(callee == NULL){
		unlock();
		return BAD_ZPID;
	}
	ZPID pid = callee->pid;
	unlock();
	return pid;
}

bool ZDLCoreImpl::attachThread(QThread *peer){
	lock();
	PluginEntry *callee = getEntryForCurrentThread();
	if(callee == NULL){
		unlock();
		return false;
	}
	threads.insert(peer, callee->pid);
	unlock();
	return true;
}

bool ZDLCoreImpl::detatchThread(QThread *peer){
	PluginEntry *callee = getEntryForCurrentThread();
	if(callee == NULL){
		unlock();
		return false;
	}
	if(!threads.contains(peer)){
		unlock();
		return false;
	}
	threads.remove(peer);
	unlock();
	return true;
}

unsigned int ZDLCoreImpl::getSequence(){
	return 0;
}

void ZDLCoreImpl::registerGuiThread(){
	if(guiThread != NULL){
		return;
	}
	QThread *currentThread = QThread::currentThread();
	guiThread = currentThread;
}

bool ZDLCoreImpl::isGuiThread(){
	QThread *currentThread = QThread::currentThread();
	return (currentThread == guiThread);
}

ZPID ZDLCoreImpl::getPidForService(QString service){
	lock();
	if(!services.contains(service)){
		qDebug() << "No service handler for " << service;
		unlock();
		return BAD_ZPID;
	}
	ZDLServiceList* serviceList = services.value(service);
	ZPID pid = serviceList->getPid();
	unlock();
	return pid;
}

bool ZDLCoreImpl::getAllPidsForServices(QList<ZPID> &list){
	return false;
}

bool ZDLCoreImpl::registerServiceHandler(QString service){
	ZPID pid = getCurrentZPID();
	if(pid == BAD_ZPID){
		return false;
	}
	lock();
	//QHash<QString, ZDLServiceList*> services;	
	ZDLServiceList* serviceList = NULL;
	if(services.contains(service)){
		serviceList = services.value(service);
	}else{
		serviceList = new ZDLServiceList(service);
		if(serviceList){
			services.insert(service,serviceList);
		}
	}
	if(!serviceList){
		unlock();
		return false;
	}

	serviceList->addHandler(pid);

	unlock();
	return true;
}

bool ZDLCoreImpl::deregisterServiceHandler(QString service){
	ZPID pid = getCurrentZPID();
	if(pid == BAD_ZPID){
		return false;
	}
	lock();
	ZDLServiceList* serviceList = NULL;
	if(!services.contains(service)){
		// Pretend we actually removed them
		unlock();
		return true;
	}
	serviceList = services.value(service);
	if(!serviceList){
		unlock();
		return false;
	}

	serviceList->removeHandler(pid);

	//TODO: Delete empty service lists

	unlock();
	return true;
}

bool ZDLCoreImpl::getAllServices(QStringList &list){
	return false;
}

bool ZDLCoreImpl::runService(ZPID pid, QString service, QHash<QString, QVariant> &payload){
	qDebug() << "ZDLCoreImpl::runService";
	if(pid == BAD_ZPID || pid == 0){
		qDebug() << "Resolving a plugin";
		pid = getPidForService(service);
	}
	if(pid == BAD_ZPID){
		qDebug() << "No plugin";
		return false;
	}
	lock();
	if(!plugins.contains(pid)){
		qDebug() << "No such plugin " << pid;
		unlock();
		return false;
	}
	PluginEntry *pe = plugins.value(pid);
        if(pe == NULL){
		qDebug() << "No such plugin entry";
                unlock();
                return false;
        }
	ZDLPluginApi *plugin = pe->plugin;
	unlock();
	qDebug() << "Running service handler";
	return plugin->handleService(service,payload);
}


ZDLServiceList::ZDLServiceList(QString serviceName){
	this->serviceName = serviceName;
	preferred = BAD_ZPID;
}

void ZDLServiceList::addHandler(ZPID pid, bool preferred){
	if(!serviceHandlers.contains(pid)){
		serviceHandlers.append(pid);
	}
	if(preferred || this->preferred == BAD_ZPID){
		this->preferred = pid;
	}
}

void ZDLServiceList::removeHandler(ZPID pid){
	if(!serviceHandlers.contains(pid)){
		return;
	}
	serviceHandlers.removeOne(pid);
	if(this->preferred = pid){
		if(serviceHandlers.isEmpty()){
			this->preferred = BAD_ZPID;
		}else{
			this->preferred = serviceHandlers.last();
		}
	}
}

ZPID ZDLServiceList::getPid(){
	if(getPreferred() != BAD_ZPID){
		return getPreferred();
	}
	if(serviceHandlers.size() == 0){
		return BAD_ZPID;
	}
	return serviceHandlers.last();
}

ZPID ZDLServiceList::getPreferred(){
	return preferred;
}

void ZDLServiceList::setPreferred(ZPID pid){
	if(!serviceHandlers.contains(pid)){
		return;
	}
	this->preferred = pid;
}


