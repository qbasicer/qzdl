#include "ZDLFileAssociations.h"
#include "zdlcommon.h"

class ZDLButtonGroup {
	public:
		ZDLButtonGroup(QString labelText, QWidget *parent){
			this->parent = parent;
			this->labelText = labelText;
		}
		bool isApply(){return assoc->isChecked();}
		bool isRemove(){return deassoc->isChecked();}
		bool isIgnore(){return ignore->isChecked();}
		void add(QVBoxLayout *layout){
			QHBoxLayout *hlayout = new QHBoxLayout();
			QButtonGroup *grpZDLFiles = new QButtonGroup(parent);
			QString text = "<b>";
			text += labelText += "</b>";
			layout->addWidget(new QLabel(text, parent));
			assoc = new QRadioButton("Associate with ZDL", parent);
			deassoc = new QRadioButton("Remove association", parent);
			ignore = new QRadioButton("Don't change association", parent);
			hlayout->addWidget(assoc);
			hlayout->addWidget(deassoc);
			hlayout->addWidget(ignore);
			grpZDLFiles->addButton(assoc);
			grpZDLFiles->addButton(deassoc);
			grpZDLFiles->addButton(ignore);
			ignore->setChecked(true);
			deassoc->setEnabled(false);
			layout->addLayout(hlayout);
			QFrame *hrTop = new QFrame(parent);
			hrTop->setFrameStyle(QFrame::HLine);
			layout->addWidget(hrTop);
		}
	protected:
		QString labelText;
		QWidget *parent;
		QRadioButton *assoc;
		QRadioButton *deassoc;
		QRadioButton *ignore;

};

// Dirty ugly hack.  Don't do this.
ZDLButtonGroup *zdlGrp;
ZDLButtonGroup *wadGrp;
ZDLButtonGroup *pk3Grp;
ZDLButtonGroup *zipGrp;
ZDLButtonGroup *patchGrp;

ZDLFileAssociations::ZDLFileAssociations(QWidget *parent):QDialog(parent){
	LOGDATAO() << "New ZDLFileAssociations" << endl;
	setWindowTitle(ZDL_ENGINE_NAME " File Associations");
	QVBoxLayout *layout = new QVBoxLayout(this);

	zdlGrp = new ZDLButtonGroup("ZDL Saved configs (*.zdl)", this);
	wadGrp = new ZDLButtonGroup("WAD Files (*.wad)", this);
	pk3Grp = new ZDLButtonGroup("PK3 Files (*.pk3)", this);
	zipGrp = new ZDLButtonGroup("ZIP Files (*.zip)", this);
	patchGrp = new ZDLButtonGroup("Patch Files (*.deh, *.bex)", this);

	zdlGrp->add(layout);
	wadGrp->add(layout);
	pk3Grp->add(layout);
	zipGrp->add(layout);
	patchGrp->add(layout);	

	QPushButton *btn = new QPushButton("OK", this);
	layout->addWidget(btn);
#if !defined(ASSOCIATE_FILETYPES_AVAILBLE)
	btn->setEnabled(false);
#endif
	connect(btn, SIGNAL(clicked()), this, SLOT(applyChecks()));
#if !defined(ASSOCIATE_FILETYPES_AVAILBLE)
	layout->addWidget(new QLabel("No support for file associations on your platform (yet)", this));
#endif


}

ZDLFileAssociations::~ZDLFileAssociations(){
	LOGDATAO() << "Destroying dialog assets" << endl;
	delete zdlGrp;
	delete wadGrp;
	delete pk3Grp;
	delete zipGrp;
	delete patchGrp;
}

void ZDLFileAssociations::applyChecks(){
#if defined(Q_WS_WIN)
	QString exec = ZDLConfigurationManager::getExec();
	exec.replace("/","\\");
	if(patchGrp->isApply()){
		LOGDATAO() << "Registering .ZDL" << endl;
		RegisterFileTypeQt(".zdl","ZDL.SaveFile","ZDL Saved Configuration File",exec,"\"%1\"",1);
	}
	if(wadGrp->isApply()){
		LOGDATAO() << "Registering .wad" << endl;
		RegisterFileTypeQt(".wad","ZDL.WADFile","Doom Engine Data File",exec,"\"%1\"",2);
	}
	if(pk3Grp->isApply()){
		LOGDATAO() << "Registering .pk3" << endl;
		RegisterFileTypeQt(".pk3","ZDL.PK3File","Doom Engine Data File",exec,"\"%1\"",2);
	}
	if(zipGrp->isApply()){
		LOGDATAO() << "Registering .zip" << endl;
		RegisterFileTypeQt(".zip","ZDL.ZipFile","Doom Engine Data File",exec,"\"%1\"",2);
	}
	if(patchGrp->isApply()){
		LOGDATAO() << "Registering .deh" << endl;
		RegisterFileTypeQt(".deh","ZDL.PatchFile","DeHackEd Patch",exec,"\"%1\"",3);
		LOGDATAO() << "Registering .bex" << endl;
		RegisterFileTypeQt(".bex","ZDL.PatchFile","DeHackEd Patch",exec,"\"%1\"",3);
	}
#else
	QMessageBox::information(this, ZDL_ENGINE_NAME, "Turns out there's no support for file associations on your platform, sorry");
#endif
	accept();
}

