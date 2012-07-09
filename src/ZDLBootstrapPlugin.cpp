#include <iostream>
#include "ZDLBootstrapPlugin.h"
#include "ZDLUIPlugin.h"
#include "ZDLCoreImpl.h"
using namespace std;

ZDLBootstrapPlugin::ZDLBootstrapPlugin(QStringList arguments){
	cout << "BootStrap Plugin" << endl;
}
int ZDLBootstrapPlugin::pluginMain(ZDLCoreApi* api){
	cout << "Bootstrap running" << endl;
	ZDLCoreImpl *impl = (ZDLCoreImpl*)api;
	ZDLUIPlugin *uiPlugin = new ZDLUIPlugin();
	ZPID pz = impl->registerPlugin(uiPlugin);
	if(pz == BAD_ZPID){
		qDebug() << "Failed to register bootstrap";
		return -1;
	}
	api->waitForProcessExit(pz);
	qDebug() << "UI died, we can exit now";
	return 0;
}
