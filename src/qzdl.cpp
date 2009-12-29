#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include <QDir>

#include "configurationManager.h"
#include "mainWindow.h"

QApplication *qapp;
QString versionString;
mainWindow *mw;

int main( int argc, char **argv ){
    QApplication a( argc, argv );
	qapp = &a;


	versionString = "3.0.5.3q";
	
	QDir cwd = QDir::current();
	configurationManager::init();
	configurationManager::setCurrentDirectory(cwd.absolutePath().toStdString());

	ZUpdater *zup = new ZUpdater();
	
	ZDLConf* tconf = new ZDLConf();
	if (argc == 2){
		configurationManager::setConfigFileName(argv[1]);
	}else{
		configurationManager::setConfigFileName("zdl.ini");
	}
	tconf->readINI(configurationManager::getConfigFileName().toStdString().c_str());
	configurationManager::setActiveConfiguration(tconf);
	
	mw = new mainWindow();
	mw->setUpdater(zup);
	mw->show();
	QObject::connect(&a, SIGNAL(lastWindowClosed()), &a, SLOT(quit()));
	mw->startRead();
	
	zup->fetch();
	
    int ret = a.exec();
	if (ret != 0){
		return ret;
	}
	mw->writeConfig();
	QString qscwd = configurationManager::getCurrentDirectory();
	tconf = configurationManager::getActiveConfiguration();
	QDir::setCurrent(qscwd);
	tconf->writeINI(configurationManager::getConfigFileName().toStdString().c_str());
	return ret;
}

