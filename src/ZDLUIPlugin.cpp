#include <iostream>
#include "ZDLUIPlugin.h"
#include "ZDLMainWindow.h"

using namespace std;

extern int uiMain(QStringList args);
extern ZDLMainWindow *mw;
QMutex *windowMutex;

ZDLUIPlugin::ZDLUIPlugin(){
	cout << "UI Plugin" << endl;
	windowMutex = new QMutex();
}
int ZDLUIPlugin::pluginMain(ZDLCoreApi* api){
	windowMutex->lock();
	cout << "UI running" << endl;
	QList<QVariant> args;
	args.append(api->getArgs());
	cout << "UI calling gui" << endl;
	api->runFunctionInGui("uiMain", args, false);
	cout << "UI done creating" << endl;
	windowMutex->lock();
	cout << "UI died, UI plugin about to die" << endl;
	return 0;
}


QVariant ZDLUIPlugin::pluginCall(QString func, QList<QVariant> args){
	qDebug() << "ZDLUIPlugin::pluginCall";
	if(func == "uiMain"){
		uiMain(args[0].toStringList());
		return QVariant();
	}
	if(func.compare(QString("addTab")) == 0){
		QString a = QString(args[0].toString());
		QWidget *b = args[1].value<QWidget*>();
		if(a.isNull() || b == NULL){
			qDebug() << "Bad values to addTab";
			return QVariant();
		}
		mw->addTab(QString(a),b);
		return QVariant();
	}
	qDebug() << "*WARNING - unknown GUI function " << func;
	return QVariant();
}
