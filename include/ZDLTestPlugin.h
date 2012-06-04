#ifndef _ZDLTESTPLUGIN_H_
#define _ZDLTESTPLUGIN_H_

#include "ZDLPluginApi.h"
#include <QtGui>

class ZDLTestPlugin : public ZDLPluginApi {
	public:
		ZDLTestPlugin();
                virtual QString getPluginName(){return QString("Test Plugin");}
                virtual QString getPluginFQDN(){return QString("net.vectec.zdl.test");}
                virtual QString getPluginCreator(){return QString("Cody Harris");}
                virtual int getPluginVersion(){return 1;}
                virtual int pluginMain(ZDLCoreApi* api);
                virtual QVariant pluginCall(QString func, QList<QVariant> args);
                virtual void deliverEvent(int evtid){Q_UNUSED(evtid);return;}
	protected:
		QWidget *createGui();
		QWidget *gui;
};


#endif

