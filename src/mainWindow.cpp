#include <iostream>
#include <QtGui>
#include <QApplication>
#include <QMainWindow>

#include "zdlInterface.h"
#include "mainWindow.h"
#include "configurationManager.h"
#include "ZInfoBar.h"

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
	windowTitle += versionString + " - " + configurationManager::getConfigFileName();
	setWindowTitle(windowTitle);
	
	setWindowIcon(configurationManager::getIcon());
	
	
	setContentsMargins(2,2,2,2);
	layout()->setContentsMargins(2,2,2,2);
	QTabWidget *widget = new QTabWidget(this);
	
 	intr = new zdlInterface(this);
	settings = new zSettingsPane(this);
	
	setCentralWidget(widget);
	widget->addTab(intr, "Main");
	widget->addTab(settings, "Settings");
	//I haven't started on this yet :)
	//widget->addTab(new zdlInterface(this), "Notifications");
	
	QAction *qact = new QAction(widget);
	qact->setShortcut(Qt::Key_Return);
	widget->addAction(qact);
	connect(qact, SIGNAL(triggered()), this, SLOT(launch()));
	
	qact2 = new QAction(widget);
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
	writeConfig();
	close();
}

void mainWindow::launch(){
	writeConfig();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	
	QString exec = getExecutable();
	if (exec.length() < 1){
		QMessageBox::critical(this, "qZDL", "Please select a source port");
		return;
	}
	QStringList args = getArguments();
	if (args.join("").length() < 1){
		return;
	}
	
	QProcess *proc = new QProcess(this);
	proc->setProcessChannelMode(QProcess::ForwardedChannels);
	proc->start(exec, args);
	int stat;
	if (zconf->hasValue("zdl.general", "autoclose")){
		QString append = zconf->getValue("zdl.save", "autoclose",&stat);
		if (append == "true"){
			close();
		}
	}
	
	procerr = proc->error();
	
	if(proc->state() != QProcess::NotRunning){
		std::cout << "ERROR!" << std::endl;
		configurationManager::setInfobarMessage("The process ended abnormally.",1);
		ZInfoBar *bar = (ZInfoBar*)configurationManager::getInfobar();
		connect(bar,SIGNAL(moreclicked()),this,SLOT(badLaunch()));
	}
	
}

void mainWindow::badLaunch(){
	if(procerr == QProcess::FailedToStart){
		QMessageBox::warning(NULL,"Failed to Start", "Failed to launch the application executable.",QMessageBox::Ok,QMessageBox::Ok);
	}else if(procerr == QProcess::Crashed){
		QMessageBox::warning(NULL,"Process Crashed", "The application ended abnormally (usually due to a crash or error).",QMessageBox::Ok,QMessageBox::Ok);
	}else{
		QMessageBox::warning(NULL,"Unknown error", "There was a problem running the application.",QMessageBox::Ok,QMessageBox::Ok);
	}
}

QStringList mainWindow::getArguments(){
	QStringList ourString;
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = NULL;
	
	unsigned int iwadIndex = 0;
	int stat;
	if(zconf->hasValue("zdl.save", "iwad")){
		int index = 0;
		string rc = zconf->getValue("zdl.save", "iwad", &stat);
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
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			if (i == iwadIndex){
				ourString << "-iwad";
				ourString << fileVctr[i]->getValue();
			
			}
		}
	}
	
	if (zconf->hasValue("zdl.save", "skill")){
		ourString << "-skill";
		ourString << zconf->getValue("zdl.save", "skill", &stat);
	}
	
	if (zconf->hasValue("zdl.general", "alwaysadd")){
		ourString << zconf->getValue("zdl.general", "alwaysadd", &stat);
	}
	
	if (zconf->hasValue("zdl.save", "warp")){
		ourString << "-warp";
		ourString << zconf->getValue("zdl.save", "warp", &stat);
	}
	
	if (zconf->hasValue("zdl.save", "dmflags")){
		ourString << "+dmflags";
		ourString << zconf->getValue("zdl.save", "dmflags", &stat);
	}
	
	if (zconf->hasValue("zdl.save", "dmflags2")){
		ourString << "+dmflags2";
		ourString << zconf->getValue("zdl.save", "dmflags2", &stat);
	}
	
	section = zconf->getSection("zdl.save");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^file[0-9]+$", fileVctr);
		
		if (fileVctr.size() > 0){
			ourString << "-file";
			for(unsigned int i = 0; i < fileVctr.size(); i++){
				ourString << fileVctr[i]->getValue();
			}
		}
	}
	
	if (zconf->hasValue("zdl.save", "extra")){
		ourString << zconf->getValue("zdl.save", "extra", &stat);
	}
	
	if(zconf->hasValue("zdl.net","advenabled")){
		QString aNetEnabled = zconf->getValue("zdl.net","advenabled",&stat);
		if(aNetEnabled == "enabled"){
			if(zconf->hasValue("zdl.net","port")){
				ourString << "-port";
				ourString << zconf->getValue("zdl.net","port",&stat);
			}
			if(zconf->hasValue("zdl.net","extratic")){
				QString tExtratic = zconf->getValue("zdl.net","extratic",&stat);
				if(tExtratic == "enabled"){
					ourString << "-extratic";
				}
			}
			if(zconf->hasValue("zdl.net","netmode")){
				QString tNetMode = zconf->getValue("zdl.net","netmode",&stat);
				if(tNetMode == "1"){
					ourString << "-netmode";
					ourString << "0";
				}else if(tNetMode == "2"){
					ourString << "-netmode";
					ourString << "1";
				}
			}
			if(zconf->hasValue("zdl.net","dup")){
				QString tDup = zconf->getValue("zdl.net","dup",&stat);
				if(tDup != "0"){
					ourString << "-dup";
					ourString << tDup;
				}
			}
		}
	}

	return ourString;
}

QString mainWindow::getExecutable(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	int stat;
	unsigned int portIndex = 0;
	if(zconf->hasValue("zdl.save", "port")){
		int index = 0;
		string rc = zconf->getValue("zdl.save", "port", &stat);
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
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
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
	settings->startRead();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	zconf->setValue("zdl.general", "engine", ZDL_ENGINE_NAME);
	zconf->setValue("zdl.general", "version", ZDL_VERSION_STRING);
	
	QString windowTitle = "ZDLSharp ";
	windowTitle += versionString + " - " + configurationManager::getConfigFileName();
	setWindowTitle(windowTitle);
}

void mainWindow::writeConfig(){
	intr->writeConfig();
	settings->writeConfig();
}
