#ifndef _ZDLBOOTSTRAPPLUGIN_H_
#define _ZDLBOOTSTRAPPLUGIN_H_

#include "ZDLPluginApi.h"

class ZDLBootstrapPlugin : public ZDLPluginApi {
	public:
		ZDLBootstrapPlugin(QStringList arguments);
		virtual QString getPluginName(){return QString("BootstrapPlugin");}
		virtual QString getPluginFQDN(){return QString("net.vectec.zdl.bootstrap");}
		virtual QString getPluginCreator(){return QString("Cody Harris");}
		virtual int getPluginVersion(){return 1;}
		virtual int pluginMain(ZDLCoreApi* api);
		virtual QVariant pluginCall(QString func, QList<QVariant> args){Q_UNUSED(func);Q_UNUSED(args);return QVariant();}
		virtual void deliverEvent(int evtid){Q_UNUSED(evtid);return;}

};

#endif
