#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "zdlInterface.h"
#include "mainWindow.h"
#include "configurationManager.h"

extern QApplication *qapp;
extern QString versionString;

mainWindow::~mainWindow(){
	delete zup;
}

void mainWindow::manageUpdate(){
	if (zup->hasUpdate()){
		QMessageBox::information(this, "qZDL Updater", "There is an update.\n\nPlease visit zdlsharp.vectec.net");
	}
}

void mainWindow::setUpdater(ZUpdater *zup){
	this->zup = zup;
	connect(zup, SIGNAL(updateReady()), this, SLOT(manageUpdate()));
}

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
	
	QAction *qact = new QAction(widget);
	qact->setShortcut(Qt::Key_Return);
	widget->addAction(qact);
	connect(qact, SIGNAL(triggered()), this, SLOT(launch()));
	
	QAction *qact2 = new QAction(widget);
	qact2->setShortcut(Qt::Key_Escape);
	widget->addAction(qact2);
	connect(qact2, SIGNAL(triggered()), this, SLOT(quit()));
	
	connect(widget, SIGNAL(currentChanged(int)), this, SLOT(tabChange(int)));
}

void mainWindow::tabChange(int newTab){
	if(newTab == 0){
		settings->notifyFromParent(NULL);
		intr->readFromParent(NULL);
	}else if (newTab == 1){
		intr->notifyFromParent(NULL);
		settings->readFromParent(NULL);
	}
}

void mainWindow::quit(){
	close();
}

void mainWindow::launch(){
	writeConfig();
	
	QString exec = getExecutable();
	if (exec.length() < 1){
		QMessageBox::critical(this, "qZDL", "Please select a source port");
		return;
	}
	QStringList args = getArguments();
	if (args.join("").length() < 1){
		return;
	}
	
	QString msg = exec;
	exec += "\n";
	exec += args.join(" ");
	QMessageBox::information(this, "qZDL", exec);
	
	//QProcess::execute(
	close();
}

QStringList mainWindow::getArguments(){
	QStringList ourString;
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = NULL;
	
	int iwadIndex = 0;
	
	if(zconf->hasValue("zdl.save", "iwad")){
		int index = 0;
		string rc = zconf->getValue("zdl.save", "iwad");
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0){
			iwadIndex = index;
		}else{
			QMessageBox::critical(this, "qZDL", "Please select an IWAD");
			return ourString;
		}
	}else{
		QMessageBox::critical(this, "qZDL", "Please select an IWAD");
		return ourString;
	}
	
	section = zconf->getSection("zdl.iwads");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^i[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			if (i == iwadIndex){
				QString append = "-iwad ";
				append += fileVctr[i]->getValue();
				ourString << append;
			}
		}
	}
}

QString mainWindow::getExecutable(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	
	int portIndex = 0;
	if(zconf->hasValue("zdl.save", "port")){
		int index = 0;
		string rc = zconf->getValue("zdl.save", "port");
		if (rc.length() > 0){
			index = atoi((char*)rc.c_str());
		}
		if (index >= 0){
			portIndex = index;
		}else{
			zconf->setValue("zdl.save", "port", 0);
			portIndex = 0;
		}
	}else{
		return "";
	}
	
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			if (i == portIndex){
				return fileVctr[i]->getValue();
			}
			
		}
	}
	return "";
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
