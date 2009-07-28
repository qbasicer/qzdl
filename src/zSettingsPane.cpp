#include <QtGui>
#include <QApplication>

#include "configurationManager.h"
#include "zListWidget.h"
#include "zSettingsPane.h"

zSettingsPane::zSettingsPane(QWidget *parent): ZQWidget(parent){
	QVBoxLayout *sections = new QVBoxLayout(this);
	
	QVBoxLayout *iwadl = new QVBoxLayout();
	QVBoxLayout *spl = new QVBoxLayout();
	
	alwaysArgs = new QLineEdit(this);
	
	QHBoxLayout *lrpane = new QHBoxLayout();
	
	//IWAD
	QVBoxLayout *lpane = new QVBoxLayout();
	iwadList = new ZIWadList(this);
	iwadl->addWidget(new QLabel("IWADs:", this));
	iwadl->addWidget(iwadList);
	
	lpane->addLayout(iwadl);
	
	//Source Port
	QVBoxLayout *rpane = new QVBoxLayout();
	sourceList = new ZSPList(this);
	spl->addWidget(new QLabel("Source Ports/Engines:", this));
	spl->addWidget(sourceList);
	
	rpane->addLayout(spl);
	
	//Add the left and right panes
	lrpane->addLayout(lpane);
	lrpane->addLayout(rpane);
	
	//Add all the sections together
	sections->addWidget(new QLabel("Always Add These Parameters", this));
	sections->addWidget(alwaysArgs);
	sections->addLayout(lrpane);
	
	updater = new QCheckBox("Enable Update Notifier", this);
	sections->addWidget(updater);
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);
}

void zSettingsPane::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.net");
	//Delete old configs
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^updateManager$", vctr);
		for(unsigned int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	if (updater->checkState() == Qt::Unchecked){
		zconf->setValue("zdl.net", "updateManager", "disabled");
	}
	
}

void zSettingsPane::newConfig(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.net");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);
		
		for(unsigned int i = 0; i < fileVctr.size(); i++){
			if (strcmp(fileVctr[i]->getValue(), "disabled") == 0){
				updater->setCheckState(Qt::Unchecked);
			}else{
				updater->setCheckState(Qt::Checked);
			}
		}
		//Make sure that it's on by default, if it's not listed
		if (fileVctr.size() < 1){
			updater->setCheckState(Qt::Checked);
		}
	}else{
		//Default to on if it's not listed
		updater->setCheckState(Qt::Checked);
	}
	
}

