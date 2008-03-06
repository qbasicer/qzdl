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
 	intr = new zdlInterface(this);
	setCentralWidget(intr);
	
}

void mainWindow::startRead(){
	intr->startRead();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	zconf->setValue("zdl.general", "engine", ZDL_ENGINE_NAME);
	zconf->setValue("zdl.general", "version", ZDL_VERSION_STRING);
}

void mainWindow::writeConfig(){
	intr->writeConfig();
}
