#include <iostream>
#include "ZDLBootstrapPlugin.h"

using namespace std;

ZDLBootstrapPlugin::ZDLBootstrapPlugin(QStringList arguments){
	cout << "BootStrap Plugin" << endl;
}
int ZDLBootstrapPlugin::pluginMain(ZDLCoreApi* api){
	cout << "Bootstrap running" << endl;
	return 0;
}
