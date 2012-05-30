#ifndef _ZDLPLUGINRUNNER_H_
#define _ZDLPLUGINRUNNER_H_

class ZDLPluginRunner;

#include <QtCore>
#include "ZDLCoreImpl.h"

class ZDLPluginRunner : public QThread {
	public:
		ZDLPluginRunner(ZDLCoreImpl *api, ZDLPluginApi *plugin);
		void run();
		int getResult(){return result;}
	private:
		ZDLCoreImpl *api;
		ZDLPluginApi *plugin;
		int result;
};

#endif

