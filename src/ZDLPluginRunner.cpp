#include "ZDLPluginRunner.h"

ZDLPluginRunner::ZDLPluginRunner(ZDLCoreApi *api, ZDLPluginApi *plugin){
	this->api = api;
	this->plugin = plugin;
}

void ZDLPluginRunner::run(){
	//Set up TLS
	result = plugin->pluginMain(api);
}

