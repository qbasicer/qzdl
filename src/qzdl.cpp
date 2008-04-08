#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QDir>

#include "configurationManager.h"
#include "mainWindow.h"

QApplication *qapp;
QString versionString;


int main( int argc, char **argv ){
    QApplication a( argc, argv );
	qapp = &a;


	versionString = "3.0.5.3q";
	
	QDir cwd = QDir::current();
	configurationManager::init();
	configurationManager::setCurrentDirectory(cwd.absolutePath().toStdString());

	ZDLConf* tconf = new ZDLConf();
	if (argc == 1){
		tconf->readINI(argv[1]);
	}else{
		tconf->readINI("zdl.ini");
	}
	configurationManager::setActiveConfiguration(tconf);
	
	mainWindow *mw = new mainWindow();
	mw->show();
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	mw->startRead();
    int ret = a.exec();
	if (ret != 0){
		return ret;
	}
	mw->writeConfig();
	QString qscwd = configurationManager::getCurrentDirectory();
	tconf = configurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	tconf->writeINI("zdl.ini");
	return ret;
}

