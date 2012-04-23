#ifndef _ZDLPLUGINAPI_H_
#define _ZDLPLUGINAPI_H_

class ZDLPluginApi {
	public:
		virtual QString getPluginName() = 0;
		virtual QString getPluginCreator() = 0;
		virtual int getPluginVersion() = 0;
		virtual int pluginMain(ZDLPluginCoreApi *api) = 0;
		virtual QVariant pluginCall(QString func, QVector<QVariant> args) = 0;
		virtual void deliverEvent(int eventid) = 0;
		
};


#endif

