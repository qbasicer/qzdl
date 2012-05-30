#include "ZDLPluginRunner.h"

ZDLPluginRunner::ZDLPluginRunner(ZDLCoreImpl *api, ZDLPluginApi *plugin){
	this->api = api;
	this->plugin = plugin;
}

void ZDLPluginRunner::run(){
	//Set up TLS
	api->fireInternalEvent(CORE_EVENT_PLG_START, (void*)plugin);
	result = plugin->pluginMain(api);
	api->fireInternalEvent(CORE_EVENT_PLG_STOP, (void*)plugin);
}

