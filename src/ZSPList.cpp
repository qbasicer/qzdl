#include "ZSPList.h"
#include "ZNameListable.h"
#include "configurationManager.h"
#include "ZNameInput.h"

#include <iostream>
using namespace std;

ZSPList::ZSPList(ZQWidget *parent): zListWidget(parent){
}

void ZSPList::newConfig(){
	pList->clear();
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^p[0-9]+f$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			string value = fileVctr[i]->getVariable();
			
			string number = "^p";
			number.append(value.substr(1, value.length()-2));
			number.append("n$");
			
			vector <ZDLLine*> nameVctr;
			section->getRegex(number.c_str(), nameVctr);
			if (nameVctr.size() == 1){
				ZNameListable *zList = new ZNameListable(pList, 1001, fileVctr[i]->getValue(), nameVctr[0]->getValue());
				insert(zList, -1);
			}
		}
	}
}


void ZSPList::rebuild(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.ports");
	if (section){
		vector <ZDLLine*> vctr;
		section->getRegex("^p[0-9]f$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
		vctr.clear();
		section->getRegex("^p[0-9]n$", vctr);
		for(int i = 0; i < vctr.size(); i++){
			section->deleteVariable(vctr[i]->getVariable());
		}
	}
	
	for(int i = 0; i < count(); i++){
		QListWidgetItem *itm = pList->item(i);
		ZNameListable* fitm = (ZNameListable*)itm;
		char szBuffer[256];
		snprintf(szBuffer, 256, "p%dn", i);
		zconf->setValue("zdl.ports", szBuffer, fitm->getName());
		snprintf(szBuffer, 256, "p%df", i);
		zconf->setValue("zdl.ports", szBuffer, fitm->getFile());
		
	
	}
	
}

void ZSPList::addButton(){
	QStringList filters;
	filters << "Executable (*.exe *.bin)"
			<< "Any files (*)";
	
	
	ZNameInput diag(this);
	diag.setWindowTitle("Add Source Port/Engine");
	diag.setFilter(filters);
	if (diag.exec()){
		ZNameListable *zList = new ZNameListable(pList, 1001, diag.getFile(), diag.getName());
		insert(zList, -1);
	}
	
	
}

