#include "ZFileList.h"
#include "ZFileListable.h"
#include "configurationManager.h"

#include <iostream>
using namespace std;

ZFileList::ZFileList(ZQWidget *parent): zListWidget(parent){
}


void ZFileList::rebuild(){
	cout << "Foo" << endl;
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");
	if (section){
		cout << "Building section config" << endl;
	}else{
		cout << "Building sectionless config of "<<count()<< endl;
		for(int i = 0; i < count(); i++){
			QListWidgetItem *itm = pList->item(i);
			ZFileListable* fitm = (ZFileListable*)itm;
			char szBuffer[256];
			snprintf(szBuffer, 256, "file%d", i);
			zconf->setValue("zdl.save", szBuffer, fitm->getFile());
		}
	}
	
}

void ZFileList::addButton(){
	QStringList filters;
	filters << "WAD/PK3/ZIP (*.wad *.pk3 *.zip)"
         << "WAD Files (*.wad)"
         << "pk3 Files (*.pk3)"
         << "zip Files (*.zip)"
         << "Any files (*)";
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFiles);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			ZFileListable *zList = new ZFileListable(pList, 1001, fileNames[i]);
			insert(zList, -1);
		}

	}


	


}

