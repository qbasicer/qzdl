/*
 * This file is part of qZDL
 * Copyright (C) 2018  Lcferrum
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

AssocListWidget::AssocListWidget(const QString &text, QListWidget *parent, const wchar_t* c_prog_id, const wchar_t* c_desc, const wchar_t* c_exts, bool hklm, UINT icon):
	QListWidgetItem(text, parent), prog_id(c_prog_id), desc(c_desc), extensions(std::istream_iterator<std::wstring, wchar_t>(std::wistringstream(c_exts)), std::istream_iterator<std::wstring, wchar_t>()), icon(icon), hklm(hklm), remove(false)
{
	setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);

	bool initial=true;
	SimpleWFA::AssocStatus res_stat;
	foreach (const std::wstring &ext, extensions) {
		SimpleWFA::AssocStatus cur_stat=SimpleWFA::CheckAssociationStatus(hklm, true, ext.c_str(), c_prog_id, NULL);
		if (initial) {
			res_stat=cur_stat;
			initial=false;
		} else if (res_stat!=cur_stat) {
			res_stat=SimpleWFA::ASSOCIATED_DUNNO;
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

void AssocListWidget::Process(const std::wstring &file_path)
{
	bool changed=orig_state!=checkState();

	if (!remove&&changed&&checkState()==Qt::Checked) {
		foreach (const std::wstring &ext, extensions) {
			SimpleWFA::Associate(hklm, ext.c_str(), prog_id.c_str());
		}
		SimpleWFA::CreateSimpleProgID(hklm, SimpleWFA::CD_UPDATE_ALWAYS, prog_id.c_str(), file_path.c_str(), desc.c_str(), icon);
	}
	
	if (remove||(changed&&checkState()==Qt::Unchecked)) {
		foreach (const std::wstring &ext, extensions) {
			SimpleWFA::Deassociate(hklm, ext.c_str(), prog_id.c_str());
		}
	}

	if (remove) {
		SimpleWFA::RemoveProgID(hklm, prog_id.c_str());
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

	assoc_list->addItem(new AssocListWidget("ZDL files (*.zdl)", assoc_list, L"ZDL.Zdl.1", L"ZDL config file", L".zdl", hklm, IDI_SAVE));
	assoc_list->addItem(new AssocListWidget("WAD files (*.wad;*.iwad)", assoc_list, L"ZDL.Wad.1", L"Doom engine WAD file", L".wad .iwad", hklm, IDI_WAD));
	assoc_list->addItem(new AssocListWidget("Patch files (*.bex;*.deh)", assoc_list, L"ZDL.Patch.1", L"Doom engine DeHackEd patch", L".bex .deh", hklm, IDI_PATCH));
	assoc_list->addItem(new AssocListWidget("Config files (*.cfg)", assoc_list, L"ZDL.Config.1", L"Doom engine config file", L".cfg", hklm, IDI_CFG));
	assoc_list->addItem(new AssocListWidget("Specialized archives (*.pk3;*.ipk3;*.pk7;*.p7z;*.pkz)", assoc_list, L"ZDL.DoomArch.1", L"Doom engine specialized archive", L".pk3 .ipk3 .pk7 .p7z .pkz", hklm, IDI_ARCH));
	assoc_list->addItem(new AssocListWidget("Other supported archives (*.zip;*.7z)", assoc_list, L"ZDL.OtherArch.1", L"Doom engine supported archive", L".zip .7z", hklm, IDI_ARCH_RED));

	main_layout->addWidget(desc);
	main_layout->addWidget(assoc_list);
	main_layout->addWidget(chk_clear);
	main_layout->addLayout(buttons);
	main_layout->setSpacing(4);

	setContentsMargins(4,4,4,4);
	layout()->setContentsMargins(0,0,0,0);
	setFixedHeight(sizeHint().height());
	setFixedWidth(320);
	resize(320, sizeHint().height());
	
	connect(btn_ok, SIGNAL(clicked()), this, SLOT(ApplyAssociations()));
	connect(chk_clear, SIGNAL(stateChanged(int)), this, SLOT(ClearStateChanged(int)));
	connect(btn_cancel, SIGNAL(clicked()), this, SLOT(reject()));
}

void ZDLFileAssociations::ApplyAssociations()
{
	std::wstring file_path=QDir::toNativeSeparators(ZDLConfigurationManager::getExec()).toStdWString();

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