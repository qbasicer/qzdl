#include "ZAdvancedMultiplayerDialog.h"
#include "configurationManager.h"
#include <QDialogButtonBox>

ZAdvancedMultiplayerDialog::ZAdvancedMultiplayerDialog(ZQWidget *parent):QDialog(parent){
	setWindowTitle("qZDL Advanced Multiplayer Settings");
	QVBoxLayout *box = new QVBoxLayout(this);
	QFormLayout *form = new QFormLayout();
	form->setFieldGrowthPolicy(QFormLayout::AllNonFixedFieldsGrow);
	QDialogButtonBox *btnBox = new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);
	
	extratic = new QCheckBox("On/Off", this);
	netmode = new QComboBox(this);	
	portNo = new QLineEdit(this);
	dupmode = new QComboBox(this);
	enable = new QCheckBox("Yes/No", this);
		
	netmode->addItem("Not Specified");
	netmode->addItem("0 (Classic)");
	netmode->addItem("1 (Client/Server Model)");
	
	dupmode->addItem("Not Specified");
	dupmode->addItem("1");
	dupmode->addItem("2");
	dupmode->addItem("3");
	dupmode->addItem("4");
	dupmode->addItem("5");
	dupmode->addItem("6");
	dupmode->addItem("7");
	dupmode->addItem("8");
	dupmode->addItem("9");
	
	connect(btnBox, SIGNAL(accepted()), this, SLOT(save()));
	connect(btnBox, SIGNAL(rejected()), this, SLOT(close()));
	
	form->addRow(new QLabel("Extratic:",this),extratic);
	form->addRow(new QLabel("Net Mode:",this),netmode);
	form->addRow(new QLabel("Port:",this),portNo);
	form->addRow(new QLabel("Dup:",this),dupmode);
	form->addRow(new QLabel("Enable:",this),enable);
	
	QLabel *top = new QLabel("ZDL<i>Sharp</i>",this);
	QFont font;
	font.setPointSize(32);
	top->setFont(font);
	top->setAlignment(Qt::AlignHCenter);
	
	QLabel *adv = new QLabel("Advanced Multiplayer Settings", this);
	adv->setAlignment(Qt::AlignHCenter);
	
	QLabel *messageOne = new QLabel("If no port is given, the default will be used.", this);
	QLabel *messageTwo = new QLabel("Settings will only be applied to a multiplayer game and when enabled.", this);
	messageOne->setWordWrap(true);
	messageTwo->setWordWrap(true);
	
	
	box->addWidget(top);
	box->addWidget(adv);
	box->addLayout(form);
	box->addWidget(messageOne);
	box->addWidget(messageTwo);
	box->addWidget(btnBox);
	readConfig();
}

void ZAdvancedMultiplayerDialog::close(){
	done(0);
}

void ZAdvancedMultiplayerDialog::readConfig(){
	int status;
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	QString tEnabled = zconf->getValue("zdl.net","advenabled",&status);
	if(status){
		tEnabled = "disabled";
	}
	
	QString tExtratic = zconf->getValue("zdl.net","extratic",&status);
	if(status){
		tExtratic = "disabled";
	}
	
	QString tPort = zconf->getValue("zdl.net","port",&status);
	if(status){
		tPort = "";
	}
	
	QString tNetMode = zconf->getValue("zdl.net","netmode",&status);
	if(status){
		tNetMode = "0";
	}
	
	QString tDup = zconf->getValue("zdl.net","dup",&status);
	if(status){
		tDup = "0";
	}
	
	if(tEnabled == "enabled"){
		enable->setCheckState(Qt::Checked);
	}else{
		enable->setCheckState(Qt::Unchecked);
	}
	
	if(tExtratic == "enabled"){
		extratic->setCheckState(Qt::Checked);
	}else{
		extratic->setCheckState(Qt::Unchecked);
	}
	
	portNo->setText(tPort);
	
	bool ok;
	int nDup = tDup.toInt(&ok, 10);
	int nNetMode = tNetMode.toInt(&ok, 10);
	if(nDup > 9){
		nDup = 9;
	}else if(nDup <= 0){
		nDup = 0;
	}
	
	if(nNetMode > 2){
		nNetMode = 2;
	}else if(nNetMode < 0){
		nNetMode = 0;
	}
	dupmode->setCurrentIndex(nDup);
	netmode->setCurrentIndex(nNetMode);
}

void ZAdvancedMultiplayerDialog::save(){
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	if(enable->isChecked()){
		zconf->setValue("zdl.net", "advenabled", "enabled");
	}else{
		zconf->setValue("zdl.net", "advenabled", "disabled");
	}
	if(extratic->isChecked()){
		zconf->setValue("zdl.net", "extratic", "enabled");
	}else{
		zconf->setValue("zdl.net", "extratic", "disabled");
	}
	
	if(portNo->text().length() > 0){
		zconf->setValue("zdl.net", "port", portNo->text().toStdString().c_str());
	}else{
		if(zconf->hasValue("zdl.net","port")){
			zconf->deleteValue("zdl.net","port");
		}
	}
	
	
	if(dupmode->currentIndex() > 0){
		zconf->setValue("zdl.net", "dup", dupmode->currentIndex());
	}else{
		if(zconf->hasValue("zdl.net","dup")){
			zconf->deleteValue("zdl.net","dup");
		}
	}
	
	if(netmode->currentIndex() > 0){
		zconf->setValue("zdl.net", "netmode", netmode->currentIndex());
	}else{
		if(zconf->hasValue("zdl.net","netmode")){
			zconf->deleteValue("zdl.net","netmode");
		}
	}
	
	
	done(1);
}
