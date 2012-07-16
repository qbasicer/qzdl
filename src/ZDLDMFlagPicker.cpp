#include <QtGui>
#include "ZDLDMFlagPicker.h"

ZDLDMFlagPickerPlugin::ZDLDMFlagPickerPlugin(){
}

int ZDLDMFlagPickerPlugin::pluginMain(ZDLCoreApi* api){
	api->registerServiceHandler("net.vectec.zdl.qzdl.dmflagpicker");
	this->api = api;
}

QVariant ZDLDMFlagPickerPlugin::pluginCall(QString func, QList<QVariant> args){
	return QVariant();
}

bool ZDLDMFlagPickerPlugin::handleService(QString service, QHash<QString, QVariant> args){
	qDebug() << "ZDLDMFlagPickerPlugin::handleService";
	if(service == "net.vectec.zdl.qzdl.dmflagpicker"){
		ZDLDMFlagPickerDialog *dialog = NULL;
		if(args.contains("parentWidget")){
			QVariant varWidget = args.value("parentWidget");
			if(!varWidget.isNull()){
				QWidget *parent = varWidget.value<QWidget*>();
				if(!parent){
					dialog = new ZDLDMFlagPickerDialog(parent);
				}
			}
		}
		if(dialog == NULL){
			dialog = new ZDLDMFlagPickerDialog();
		}
		if(dialog == NULL){
			qDebug() << "No dialoag";
			return false;
		}
		dialog->exec();
		qDebug() << "Dialog done";
		return true;
	}
	return false;
}

ZDLDMFlagPickerDialog::ZDLDMFlagPickerDialog(QWidget *parent) : QDialog(parent){
}

