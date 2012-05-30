#include <iostream>
#include "ZDLUIPlugin.h"

using namespace std;

extern int uiMain(QStringList args);

ZDLUIPlugin::ZDLUIPlugin(){
	cout << "UI Plugin" << endl;
}
int ZDLUIPlugin::pluginMain(ZDLCoreApi* api){
	cout << "UI running" << endl;
	QVector<QVariant> args;
	args.append(api->getArgs());
	cout << "UI calling gui" << endl;
	api->runFunctionInGui(this, "uiMain", args, false);
	cout << "UI done" << endl;
	return 0;
}

QVariant ZDLUIPlugin::pluginCall(QString func, QVector<QVariant> args){
	if(func == "uiMain"){
		uiMain(args[0].toStringList());
	}
	return QVariant();
}
