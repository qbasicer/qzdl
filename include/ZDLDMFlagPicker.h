#ifndef _ZDLDMFLAGPICKER_HPP_
#define _ZDLDMFLAGPICKER_HPP_

class ZDLDMFlagPickerPlugin;
class ZDLDMFlagPickerDialog;

#include <QtGui>
#include <QtXml>
#include "ZDLPluginApi.h"


class ZDLDMFlagPickerPlugin : public ZDLPluginApi {
	public:
               	ZDLDMFlagPickerPlugin();
                virtual QString getPluginName(){return QString("ZDL DMFlag Picker");}
                virtual QString getPluginFQDN(){return QString("net.vectec.zdl.qzdl.dmflagpicker");}
                virtual QString getPluginCreator(){return QString("Cody Harris <qbasicer@gmail.com>");}
                virtual int getPluginVersion(){return 1;}
                virtual int pluginMain(ZDLCoreApi* api);
                virtual QVariant pluginCall(QString func, QList<QVariant> args);
                virtual void deliverEvent(int evtid){Q_UNUSED(evtid);return;}
                virtual bool handleService(QString, QHash<QString, QVariant>);
        protected:
                ZDLCoreApi *api;

};

class ZDLDMFlagPickerDialog : public QDialog {
	public:
		ZDLDMFlagPickerDialog(QWidget *parent = NULL);
};




#endif
