#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "zdlInterface.h"
#include "mainWindow.h"
#include "configurationManager.h"



extern QApplication *qapp;
extern QString versionString;


mainWindow::mainWindow(QWidget *parent): QMainWindow(parent){
	QString windowTitle = "ZDLSharp ";
	windowTitle += versionString;
	setWindowTitle(windowTitle);
	setContentsMargins(2,2,2,2);
	layout()->setContentsMargins(2,2,2,2);
	QTabWidget *widget = new QTabWidget(this);
	
 	intr = new zdlInterface(this);
	settings = new zSettingsPane(this);
	
	setCentralWidget(widget);
	widget->addTab(intr, "Main");
	widget->addTab(settings, "Settings");
	widget->addTab(new zdlInterface(this), "Notifications");
	
}


//Pass through functions.
void mainWindow::startRead(){
	intr->startRead();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	zconf->setValue("zdl.general", "engine", ZDL_ENGINE_NAME);
	zconf->setValue("zdl.general", "version", ZDL_VERSION_STRING);
}

void mainWindow::writeConfig(){
	intr->writeConfig();
}
