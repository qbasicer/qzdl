/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
 

#include <QtGui>
#include <QApplication>
#include <QComboBox>
#include "ZDLConfigurationManager.h"
#include "ZDLMultiPane.h"

#ifdef _DMFLAG_PICKER_
#include "ZDMFlagDialog.h"
#endif

void PlayersValidator::fixup(QString &input) const
{
	Q_UNUSED(input);
	validated_cb->setEditable(false);
	validated_cb->setCurrentIndex(0);
}

ZDLMultiPane::ZDLMultiPane(ZDLWidget *parent): ZDLWidget(parent) {
	launch_btn=NULL;

	QVBoxLayout *box = new QVBoxLayout(this);

	gMode = new QComboBox(this);
	gMode->addItem("Singleplayer");
	gMode->addItem("Co-op");
	gMode->addItem("Deathmatch");
	gMode->addItem("AltDeathmatch");
	
	max_int_validator=new QIntValidator(0, INT_MAX, this);
	
	tHostAddy = new QLineEdit(this);
	
	gPlayers = new QComboBox(this);
	gPlayers->setInsertPolicy(QComboBox::NoInsert);	
	gPlayers->addItem("Joining");
	gPlayers->addItem("1");
	gPlayers->addItem("2");
	gPlayers->addItem("3");
	gPlayers->addItem("4");
	gPlayers->addItem("5");
	gPlayers->addItem("6");
	gPlayers->addItem("7");
	gPlayers->addItem("8");
	gPlayers->addItem("(More...)");

	players_validator=new PlayersValidator(this, gPlayers);
	
	tFragLimit = new QLineEdit(this);
	tFragLimit->setValidator(max_int_validator);

	tTimeLimit = new QLineEdit(this);
	tTimeLimit->setValidator(max_int_validator);

	extratic = new QComboBox(this);	
	netmode = new QComboBox(this);	
	portNo = new QLineEdit(this);
	portNo->setValidator(new QIntValidator(0, 65535, this));
	dupmode = new QComboBox(this);
	savegame = new VerboseComboBox(this);
	savegame->setInsertPolicy(QComboBox::NoInsert);	
	savegame->setEditable(true);
	savegame->setValidator(new EvilValidator(this));
	savegame->setCompleter(NULL);
	savegame->addItem("(None)");
			
	netmode->addItem("(Default)");
	netmode->addItem("0 (Classic P2P)");
	netmode->addItem("1 (Client/Server)");

	extratic->addItem("Off (Default)");
	extratic->addItem("On");
	
	dupmode->addItem("(Default)");
	dupmode->addItem("1");
	dupmode->addItem("2");
	dupmode->addItem("3");
	dupmode->addItem("4");
	dupmode->addItem("5");
	dupmode->addItem("6");
	dupmode->addItem("7");
	dupmode->addItem("8");
	dupmode->addItem("9");
	
	//Per issue #26, remove DMFlag picker
#ifdef _DMFLAG_PICKER_
	bDMFlags = new QPushButton("0",this);
	bDMFlags2 = new QPushButton("0",this);
#else
	bDMFlags = new QLineEdit("", this);
	bDMFlags->setValidator(max_int_validator);
	bDMFlags2 = new QLineEdit("", this);
	bDMFlags2->setValidator(max_int_validator);
#endif
	
	QGridLayout *topGrid = new QGridLayout();

	topGrid->addWidget(new QLabel("Game mode",this),0,0);
	topGrid->addWidget(gMode,1,0);
	topGrid->addWidget(new QLabel("Hostname/IP",this),0,1,1,3);
	topGrid->addWidget(tHostAddy,1,1,1,3);
	topGrid->addWidget(new QLabel("Port",this),0,4);
	topGrid->addWidget(portNo,1,4);

	topGrid->addWidget(new QLabel("Players",this),2,0);
	topGrid->addWidget(gPlayers,3,0);
	topGrid->addWidget(new QLabel("Frag limit",this),2,1);
	topGrid->addWidget(tFragLimit,3,1);
	topGrid->addWidget(new QLabel("Time limit",this),2,2);
	topGrid->addWidget(tTimeLimit,3,2);
	topGrid->addWidget(new QLabel("DMFLAGS",this),2,3);
	topGrid->addWidget(bDMFlags,3,3);
	topGrid->addWidget(new QLabel("DMFLAGS2",this),2,4);
	topGrid->addWidget(bDMFlags2,3,4);

	topGrid->addWidget(new QLabel("Net mode",this),4,0);
	topGrid->addWidget(netmode,5,0);
	topGrid->addWidget(new QLabel("Dup",this),4,1);
	topGrid->addWidget(dupmode,5,1);
	topGrid->addWidget(new QLabel("Extratic",this),4,2);
	topGrid->addWidget(extratic,5,2);
	topGrid->addWidget(new QLabel("Savegame",this),4,3,1,2);
	topGrid->addWidget(savegame,5,3,1,2);

	topGrid->setColumnStretch(1, 2);
	topGrid->setColumnStretch(2, 2);
	topGrid->setColumnStretch(3, 3);
	topGrid->setColumnStretch(4, 3);

	topGrid->setSpacing(2);
	box->addLayout(topGrid);
	
	setContentsMargins(0,0,0,0);
	layout()->setContentsMargins(0,0,0,0);

#ifdef _DMFLAG_PICKER_
	connect(bDMFlags, SIGNAL(clicked()), this, SLOT(dmflags()));
	connect(bDMFlags2, SIGNAL(clicked()), this, SLOT(dmflags2()));
#endif
	connect(gMode, SIGNAL(currentIndexChanged(int)), this, SLOT(ModePlayerChanged(int)));
	connect(gPlayers, SIGNAL(currentIndexChanged(int)), this, SLOT(ModePlayerChanged(int)));

	connect(gPlayers, SIGNAL(activated(int)), this, SLOT(EditPlayers(int)));
	connect(savegame, SIGNAL(activated(int)), this, SLOT(EditSave(int)));
	connect(savegame, SIGNAL(onPopup()), this, SLOT(VerbosePopup()));
}

void ZDLMultiPane::EditPlayers(int idx)
{
	if (idx==9) {
		gPlayers->setEditable(true);
		gPlayers->setCurrentIndex(-1);
		gPlayers->setCompleter(NULL);
		gPlayers->setValidator(players_validator);
	} else {
		gPlayers->setEditable(false);
	}
}

void ZDLMultiPane::VerbosePopup()
{
	QString prev_save=savegame->currentText();
	setProperty("prev_save", prev_save);

	QFileInfo fi(prev_save);
	QString save_path;

	if (prev_save.isEmpty())
		save_path=getSaveLastDir();
	else if (fi.isAbsolute()&&fi.isFile())
		save_path=fi.absolutePath();

	savegame->setUpdatesEnabled(false);
	savegame->clear();
	savegame->addItem("(None)");
	savegame->addItem("(Browse...)");

	if (save_path.size()) {
		QStringList filter("*.zds");
		QDir save_dir(save_path);
		QFileInfoList saves=save_dir.entryInfoList(filter);

		foreach (const QFileInfo &sfi, saves) {
			if (sfi.isFile())
				savegame->addItem(sfi.fileName(), sfi.absoluteFilePath());
		}
	}

	int idx;
	if (prev_save.isEmpty()) {
		savegame->setCurrentIndex(0);
		savegame->clearEditText();
	} else if ((idx=savegame->findData(prev_save, Qt::UserRole, Qt::MatchExactly))>1) {
		savegame->setCurrentIndex(idx);
		savegame->setEditText(prev_save);
	} else {
		savegame->setCurrentIndex(-1);
		savegame->setEditText(prev_save);
	}
	savegame->setUpdatesEnabled(true);
}

void ZDLMultiPane::EditSave(int idx)
{
	savegame->setCurrentIndex(-1);
	if (idx==1) {
		QString prev_save=property("prev_save").toString();

        QString filters =
            "Savefiles (*.zds);;"
            "All files (" QFD_FILTER_ALL ")";
		QFileInfo fi(prev_save);
		QString save_path=(fi.isRelative()||!fi.isFile())?getSaveLastDir():fi.absolutePath();
        QString new_save=QFileDialog::getOpenFileName(this, "Select savefile", save_path, filters);

		if (new_save.isEmpty()) {
			savegame->setEditText(prev_save);
		} else {
			savegame->setEditText(QFD_QT_SEP(new_save));
			saveSaveLastDir(new_save);
		}
	} else if (idx>1) {
		savegame->setEditText(savegame->itemData(idx).toString());
	}
}

void ZDLMultiPane::enableAll()
{
	gPlayers->setEnabled(true);
	tFragLimit->setEnabled(true);
	extratic->setEnabled(true);
	netmode->setEnabled(true);	
	portNo->setEnabled(true);
	dupmode->setEnabled(true);
	bDMFlags2->setEnabled(true);
	bDMFlags->setEnabled(true);
	tHostAddy->setEnabled(true);
	tTimeLimit->setEnabled(true);
	savegame->setEnabled(true);
}

void ZDLMultiPane::disableAll()
{
	gPlayers->setEnabled(false);
	tFragLimit->setEnabled(false);
	extratic->setEnabled(false);
	netmode->setEnabled(false);	
	portNo->setEnabled(false);
	dupmode->setEnabled(false);
	bDMFlags2->setEnabled(false);
	bDMFlags->setEnabled(false);
	tHostAddy->setEnabled(false);
	tTimeLimit->setEnabled(false);
	savegame->setEnabled(false);
}

void ZDLMultiPane::ModePlayerChanged(int idx)
{
	Q_UNUSED(idx);

	if (gMode->currentIndex()) {
		enableAll();
		if (gPlayers->currentIndex())
			launch_btn->setText("Host");
		else
			launch_btn->setText("Join");
	} else {
		disableAll();
		launch_btn->setText("Launch");
	}
}

void ZDLMultiPane::newConfig(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.save");

	if (section && section->hasVariable("host")){
		tHostAddy->setText(section->findVariable("host"));
	}else{
		tHostAddy->setText("");
	}

	if (section && section->hasVariable("savegame")){
		savegame->setEditText(section->findVariable("savegame"));
	}else{
		savegame->clearEditText();
	}

	if (section && section->hasVariable("mp_port")){
		QString dmFlags = section->findVariable("mp_port");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok||flags<0){
			portNo->setText("");
		}else{
			portNo->setText(dmFlags);
		}
	}else{
		portNo->setText("");
	}

	{
		int new_gmode_idx=0;

		if (section&&section->hasVariable("gametype")) {
			new_gmode_idx=section->findVariable("gametype").toInt();
			if (new_gmode_idx<0||new_gmode_idx>3)
				new_gmode_idx=0;
		}

		if (new_gmode_idx==gMode->currentIndex())
			ModePlayerChanged(new_gmode_idx);
		else
			gMode->setCurrentIndex(new_gmode_idx);
	}
	{
		int new_pl_idx=0;
		int new_pl_txt=0;

		if (section&&section->hasVariable("players")) {
			new_pl_txt=new_pl_idx=section->findVariable("players").toInt();
			if (new_pl_idx<0)
				new_pl_idx=0;
			else if (new_pl_idx>8)
				new_pl_idx=-1;
		}

		if (new_pl_idx==gPlayers->currentIndex()) {
			ModePlayerChanged(new_pl_idx);
		} else {
			gPlayers->setCurrentIndex(new_pl_idx);
		}

		if (new_pl_idx>=0) {
			gPlayers->setEditable(false);			
		} else {
			gPlayers->setEditable(true);
			gPlayers->setEditText(QString::number(new_pl_txt));
			gPlayers->setCompleter(NULL);
			gPlayers->setValidator(players_validator);
		}
	}
	if (section && section->hasVariable("extratic")){
		QString strVar = section->findVariable("extratic");
		bool ok;
		int iVar = strVar.toInt(&ok, 10);
		if (iVar >= 0 && iVar <= 1 && ok){
			extratic->setCurrentIndex(iVar);
		}else{
			extratic->setCurrentIndex(0);
		}
		
	}else{
		extratic->setCurrentIndex(0);
	}
	if (section && section->hasVariable("netmode")){
		QString strVar = section->findVariable("netmode");
		bool ok;
		int iVar = strVar.toInt(&ok, 10);
		if (iVar >= -1 && iVar <= 1 && ok){
			netmode->setCurrentIndex(iVar+1);
		}else{
			netmode->setCurrentIndex(0);
		}
		
	}else{
		netmode->setCurrentIndex(0);
	}
	if (section && section->hasVariable("dup")){
		QString strVar = section->findVariable("dup");
		bool ok;
		int iVar = strVar.toInt(&ok, 10);
		if (iVar >= 0 && iVar <= 9 && ok){
			dupmode->setCurrentIndex(iVar);
		}else{
			dupmode->setCurrentIndex(0);
		}
		
	}else{
		dupmode->setCurrentIndex(0);
	}
	if (section && section->hasVariable("dmflags")){
		QString dmFlags = section->findVariable("dmflags");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok||flags<0){
			bDMFlags->setText("");
		}else{
			bDMFlags->setText(dmFlags);
		}
	}else{
		bDMFlags->setText("");
	}
	if (section && section->hasVariable("dmflags2")){
		QString dmFlags = section->findVariable("dmflags2");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok||flags<0){
			bDMFlags2->setText("");
		}else{
			bDMFlags2->setText(dmFlags);
		}
	}else{
		bDMFlags2->setText("");
	}
	
	if (section && section->hasVariable("fraglimit")){
		QString dmFlags = section->findVariable("fraglimit");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok||flags<0){
			tFragLimit->setText("");
		}else{
			tFragLimit->setText(dmFlags);
		}
	}else{
		tFragLimit->setText("");
	}

	if (section && section->hasVariable("timelimit")){
		QString dmFlags = section->findVariable("timelimit");
		bool ok;
		int flags = dmFlags.toInt(&ok, 10);
		if (!ok||flags<0){
			tTimeLimit->setText("");
		}else{
			tTimeLimit->setText(dmFlags);
		}
	}else{
		tTimeLimit->setText("");
	}
	
}

void ZDLMultiPane::rebuild(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();

	if (tHostAddy->text().length() > 0){
		zconf->setValue("zdl.save", "host", tHostAddy->text());
	}else{
		zconf->deleteValue("zdl.save", "host");
	}

	if (savegame->currentText().length() > 0){
		zconf->setValue("zdl.save", "savegame", savegame->currentText());
	}else{
		zconf->deleteValue("zdl.save", "savegame");
	}

	if (portNo->text().length() > 0){
		zconf->setValue("zdl.save", "mp_port", portNo->text());
	}else{
		zconf->deleteValue("zdl.save", "mp_port");
	}

	if (tFragLimit->text().length() > 0){
		zconf->setValue("zdl.save", "fraglimit", tFragLimit->text());
	}else{
		zconf->deleteValue("zdl.save", "fraglimit");
	}

	if (tTimeLimit->text().length() > 0){
		zconf->setValue("zdl.save", "timelimit", tTimeLimit->text());
	}else{
		zconf->deleteValue("zdl.save", "timelimit");
	}

	if(bDMFlags->text().length() > 0){
		zconf->setValue("zdl.save", "dmflags", bDMFlags->text());
	}else{
		zconf->deleteValue("zdl.save", "dmflags");
	}

	if(bDMFlags2->text().length() > 0){
		zconf->setValue("zdl.save", "dmflags2", bDMFlags2->text());
	}else{
		zconf->deleteValue("zdl.save", "dmflags2");
	}

	zconf->setValue("zdl.save", "gametype", gMode->currentIndex());
	if (gPlayers->currentIndex()==-1)
		zconf->setValue("zdl.save", "players", gPlayers->currentText().toInt());
	else
		zconf->setValue("zdl.save", "players", gPlayers->currentIndex());
	zconf->setValue("zdl.save", "extratic", extratic->currentIndex());
	zconf->setValue("zdl.save", "netmode", netmode->currentIndex()-1);
	zconf->setValue("zdl.save", "dup", dupmode->currentIndex());
}


void ZDLMultiPane::dmflags(){
	//Per issue #26, remove DMFlag picker
#if 0
	ZDMFlagDialog dialog(this);
	bool ok;
	dialog.setValue(bDMFlags->text().toInt(&ok, 10));
	dialog.setValue2(bDMFlags2->text().toInt(&ok, 10));
	int ret = dialog.exec();
	if (ret == 1){
		bDMFlags->setText(QString::number(dialog.value()));
		bDMFlags2->setText(QString::number(dialog.value2()));
	}
#endif
}

void ZDLMultiPane::dmflags2(){
	dmflags();
}











