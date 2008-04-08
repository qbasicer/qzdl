#include "ZFileList.h"
#include "ZFileListable.h"

ZFileList::ZFileList(ZQWidget *parent): zListWidget(parent){
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

