#include "ZDLTestPlugin.h"

ZDLTestPlugin::ZDLTestPlugin(){
	this->gui = NULL;
}

int ZDLTestPlugin::pluginMain(ZDLCoreApi* api){
	api->runFunctionInGui(this, "createGui", QList<QVariant>(), false);
	if(this->gui != NULL){
		qDebug() << "Adding tab";
		printf("Widget is 0x%p\n", (QWidget*)this->gui);
		api->addTab("TestPlugin", (QWidget*)this->gui);
	}
}

QVariant ZDLTestPlugin::pluginCall(QString func, QList<QVariant> args){
	if(func == "createGui"){
		this->gui = createGui();
		return qVariantFromValue(this->gui);
	}
	qDebug() << "Unhandled message " << func;
	return QVariant();
}

QWidget *ZDLTestPlugin::createGui(){
	qDebug() << "Creating GUI";
	QLabel *label = new QLabel("Testing!");
	return label;
}

