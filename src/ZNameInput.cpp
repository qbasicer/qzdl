#include <QtGui>
#include "ZNameInput.h"
#include <string>

ZNameInput::ZNameInput(QWidget *parent):QDialog(parent){
	QVBoxLayout *lays = new QVBoxLayout(this);
	QHBoxLayout *files = new QHBoxLayout();
	
	QHBoxLayout *buttons = new QHBoxLayout();
	
	QPushButton *btnOK = new QPushButton("OK", this);
	QPushButton *btnCancel = new QPushButton("Cancel", this);
	
	lname = new QLineEdit(this);
	lfile = new QLineEdit(this);
	btnBrowse = new QPushButton("Browse...", this);
	
	lays->addWidget(new QLabel("Name", this));
	lays->addWidget(lname);
	
	lays->addWidget(new QLabel("File", this));
	
	files->addWidget(lfile);
	files->addWidget(btnBrowse);
	
	lays->addLayout(files);
	
	buttons->addWidget(btnOK);
	buttons->addWidget(btnCancel);
	
	lays->addLayout(buttons);
	
	connect(btnBrowse, SIGNAL(clicked()), this, SLOT(browse()));
	connect(btnOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(btnCancel, SIGNAL(clicked()), this, SLOT(reject()));	
	
	
}

void ZNameInput::browse(){
	QFileDialog dialog(this);
	dialog.setFilters(filters);
	dialog.setFileMode(QFileDialog::ExistingFile);
	QStringList fileNames;
	if(dialog.exec()){
		fileNames = dialog.selectedFiles();
		for(int i = 0; i < fileNames.size(); i++){
			lfile->setText(fileNames[i]);
			
		}

	}
}

void ZNameInput::setFilter(QStringList inFilters){
	filters = inFilters;
}

const char* ZNameInput::getName(){
	if (lname->text().length() > 0){
		return lname->text().toStdString().c_str();
	}else{
		return lfile->text().toStdString().c_str();
	}
}

const char* ZNameInput::getFile(){
	return lfile->text().toStdString().c_str();
}
