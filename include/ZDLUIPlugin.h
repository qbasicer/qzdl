#ifndef _ZDLUIPLUGIN_H_
#define _ZDLUIPLUGIN_H_

#include "ZDLPluginApi.h"

class ZDLUIPlugin : public ZDLPluginApi {
        public:
                ZDLUIPlugin();
                virtual QString getPluginName(){return QString("UI Plugin");}
		virtual QString getPluginFQDN(){return QString("net.vectec.zdl.ui");}
                virtual QString getPluginCreator(){return QString("Cody Harris");}
                virtual int getPluginVersion(){return 1;}
                virtual int pluginMain(ZDLCoreApi* api);
                virtual QVariant pluginCall(QString func, QList<QVariant> args);
                virtual void deliverEvent(int evtid){Q_UNUSED(evtid);return;}
		virtual bool handleService(QString, QHash<QString, QVariant>&){return false;}
	protected:
		ZDLCoreApi *api;


};

#endif
