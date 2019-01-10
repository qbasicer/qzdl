//SimpleWFA - Simple Windows File Association library
//Copyright (c) 2018 Lcferrum
//This program comes with no warranty.
//You must use this program at your own risk.
//Licensed under BSD 2-Clause License - see SimpleWFA.LICENSE file for details.

#ifndef WINFILEASSOC_H
#define WINFILEASSOC_H

#include <windows.h>
#include <string>

//Typical ProgID is [Program Name].[Extension] but Microsoft actually recommends this format: [Vendor or Application].[Component].[Version]
//Also Microsoft says that ProgID limit is 39 characters, it contains only alphanumeric characters and periods and it shouldn't start with a digit
//Extensions should be fully qualified - they should include leading period (e.g. ".ext")
//If prog_open_params is specified, use "%1" (including double quotes) as filename placeholder for 'open' verse
//If prog_icon_res_id is 0, default application icon will be used
//Do not double quote prog_path and also make shure that it properly uses backslashes as path delimiter
//References:
//	https://docs.microsoft.com/en-us/windows/desktop/shell/fa-progids
//	https://docs.microsoft.com/en-us/windows/desktop/com/-progid--key

namespace SimpleWFA {
	enum AssocStatus {ASSOCIATED_AFF, ASSOCIATED_NEG, ASSOCIATED_DUNNO};
	enum CreationDisposition {CD_CREATE_NEW, CD_CREATE_ALWAYS, CD_UPDATE_ALWAYS};
	
	bool CheckIfLocalMachineAvailable();
	AssocStatus CheckAssociationStatus(bool local_machine, bool check_registration, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe=NULL);
	void Associate(bool local_machine, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe=NULL);
	void Deassociate(bool local_machine, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe=NULL);
	void CreateSimpleProgID(bool local_machine, CreationDisposition creation_disp, const wchar_t* prog_id, const wchar_t* prog_path, const wchar_t* prog_desc=NULL, UINT prog_icon_res_id=0, const wchar_t* prog_open_params=NULL);
	void RemoveProgID(bool local_machine, const wchar_t* prog_id);
	void NotifyShell();
}

#endif //WINFILEASSOC_H