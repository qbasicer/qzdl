#ifndef _ZDLPLUGINRUNNER_H_
#define _ZDLPLUGINRUNNER_H_

#include <QtCore>
#include "ZDLCoreApi.h"
#include "ZDLPluginApi.h"

class ZDLPluginRunner : public QThread {
	public:
		ZDLPluginRunner(ZDLCoreApi *api, ZDLPluginApi *plugin);
		void run();
		int getResult(){return result;}
	private:
		ZDLCoreApi *api;
		ZDLPluginApi *plugin;
		int result;
};

#endif

