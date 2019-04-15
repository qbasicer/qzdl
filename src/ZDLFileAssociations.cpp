/*
 * This file is part of qZDL
 * Copyright (C) 2007-2012  Cody Harris
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

#include <iterator>
#include <sstream>
#include "ZDLConfigurationManager.h"
#include "ZDLFileAssociations.h"
#include "SimpleWFA.h"
#include "zdlcommon.h"
#include "resource.h"

AssocListWidget::AssocListWidget(const QString &text, QListWidget *parent, const QString &prog_id, const QString &desc, const QString &exts, bool hklm, UINT icon):
	QListWidgetItem(text, parent), prog_id(prog_id), desc(desc), extensions(exts.split(" ")), icon(icon), hklm(hklm), remove(false)
{
	setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

	bool initial=true;
	SimpleWFA::AssocStatus res_stat;
	foreach (const QString &ext, extensions) {
		SimpleWFA::AssocStatus cur_stat=SimpleWFA::CheckAssociationStatus(hklm, true, ext.utf16(), prog_id.utf16(), NULL);
		if (initial) {
			res_stat=cur_stat;
			initial=false;
		} else if (res_stat!=cur_stat) {
			res_stat=SimpleWFA::ASSOCIATED_DUNNO;
			break;
		}
	}

	switch (res_stat) {
		case SimpleWFA::ASSOCIATED_AFF:
			orig_state=Qt::Checked;
			break;
		case SimpleWFA::ASSOCIATED_NEG:
			orig_state=Qt::Unchecked;
			break;
		case SimpleWFA::ASSOCIATED_DUNNO:
			orig_state=Qt::PartiallyChecked;
			break;
	}

	setCheckState(orig_state);
}

void AssocListWidget::Process(const QString &file_path)
{
	bool changed=orig_state!=checkState();

	if (!remove&&changed&&checkState()==Qt::Checked) {
		foreach (const QString &ext, extensions) {
			SimpleWFA::Associate(hklm, ext.utf16(), prog_id.utf16());
		}
		SimpleWFA::CreateSimpleProgID(hklm, SimpleWFA::CD_UPDATE_ALWAYS, prog_id.utf16(), file_path.utf16(), desc.utf16(), icon);
	}
	
	if (remove||(changed&&checkState()==Qt::Unchecked)) {
		foreach (const QString &ext, extensions) {
			SimpleWFA::Deassociate(hklm, ext.utf16(), prog_id.utf16());
		}
	}

	if (remove) {
		SimpleWFA::RemoveProgID(hklm, prog_id.utf16());
	}
}

void AssocListWidget::SetRemove(int state)
{
	remove=state;

	if (remove)
		setFlags(Qt::ItemIsUserCheckable);
	else
		setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
}

ZDLFileAssociations::ZDLFileAssociations(QWidget *parent):
	QDialog(parent)
{
	setWindowTitle("File associations");
	setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint); 
	QVBoxLayout *main_layout=new QVBoxLayout(this);
	QHBoxLayout *buttons=new QHBoxLayout();
	assoc_list=new QListWidget(this);
		
	QPushButton *btn_ok=new QPushButton("OK", this);
	QPushButton *btn_cancel=new QPushButton("Cancel", this);
	QCheckBox *chk_clear=new QCheckBox("Remove all association data", this);
	QLabel *desc=new QLabel("Associate ZDL with following file types", this);
	
	buttons->addStretch();
	buttons->addWidget(btn_ok);
	buttons->addWidget(btn_cancel);
	buttons->setSpacing(4);
	buttons->setContentsMargins(0,0,0,0);

	bool hklm=SimpleWFA::CheckIfLocalMachineAvailable();

	assoc_list->addItem(new AssocListWidget("ZDL files (*.zdl)", assoc_list, "ZDL.Zdl.1", "ZDL config file", ".zdl", hklm, IDI_SAVE));
	assoc_list->addItem(new AssocListWidget("WAD files (*.wad;*.iwad)", assoc_list, "ZDL.Wad.1", "Doom engine WAD file", ".wad .iwad", hklm, IDI_WAD));
	assoc_list->addItem(new AssocListWidget("Patch files (*.bex;*.deh)", assoc_list, "ZDL.Patch.1", "Doom engine DeHackEd patch", ".bex .deh", hklm, IDI_PATCH));
	assoc_list->addItem(new AssocListWidget("Config files (*.cfg)", assoc_list, "ZDL.Config.1", "Doom engine config file", ".cfg", hklm, IDI_CFG));
	assoc_list->addItem(new AssocListWidget("Specialized archives (*.pk3;*.ipk3;*.pk7;*.ipk7;*.p7z;*.pkz;*.pke)", assoc_list, "ZDL.DoomArch.1", "Doom engine specialized archive", ".pk3 .ipk3 .pk7 .ipk7 .p7z .pkz .pke", hklm, IDI_ARCH));
	assoc_list->addItem(new AssocListWidget("Other supported archives (*.zip;*.7z)", assoc_list, "ZDL.OtherArch.1", "Doom engine supported archive", ".zip .7z", hklm, IDI_ARCH_RED));

	main_layout->addWidget(desc);
	main_layout->addWidget(assoc_list);
	main_layout->addWidget(chk_clear);
	main_layout->addLayout(buttons);
	main_layout->setSpacing(4);

	setContentsMargins(4,4,4,4);
	layout()->setContentsMargins(0,0,0,0);
	setFixedHeight(sizeHint().height());
	setFixedWidth(400);
	resize(400, sizeHint().height());
	
	connect(btn_ok, SIGNAL(clicked()), this, SLOT(ApplyAssociations()));
	connect(chk_clear, SIGNAL(stateChanged(int)), this, SLOT(ClearStateChanged(int)));
	connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ZDLFileAssociations::ApplyAssociations()
{
	QString file_path=QDir::toNativeSeparators(ZDLConfigurationManager::getExec());

	for(int i=0; i<assoc_list->count(); i++)
		((AssocListWidget*)assoc_list->item(i))->Process(file_path);

	SimpleWFA::NotifyShell();

	accept();
}

void ZDLFileAssociations::ClearStateChanged(int state)
{
	assoc_list->setUpdatesEnabled(false);
	for(int i=0; i<assoc_list->count(); i++)
		((AssocListWidget*)assoc_list->item(i))->SetRemove(state);
	assoc_list->setUpdatesEnabled(true);
}