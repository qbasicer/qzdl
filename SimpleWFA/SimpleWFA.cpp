//SimpleWFA - Simple Windows File Association library
//Copyright (c) 2018 Lcferrum
//This program comes with no warranty.
//You must use this program at your own risk.
//Licensed under BSD 2-Clause License - see SimpleWFA.LICENSE.txt file for details.

#include <algorithm>
#include <shlobj.h>
#include "SimpleWFA.h"

#define MEETS_CONDITION		(1<<0)
#define SECURITY_ISSUE		(1<<1)
#define	KEY_NOT_FOUND		(1<<2)
#define KEY_IS_EMPTY		(1<<3)
#define VALUE_NOT_FOUND		(1<<4)
#define VALUE_NOT_SINGLE	(1<<5)
#define DATA_NOT_EQUAL		(1<<6)
#define NO_KEY_OR_VALUE		(KEY_IS_EMPTY|VALUE_NOT_FOUND|KEY_NOT_FOUND)

#define USER_CHOICE_NONE	(1<<0)
#define USER_CHOICE_MATCH	(1<<1)
#define USER_CHOICE_OTHER	(1<<2)
#define USER_CHOICE_AMBIG	(1<<3)
#define USER_CHOICE_APPEXE	(1<<4)
#define USER_CHOICE_ERROR	(1<<5)

#define DEFAULT_VALUE		L""

namespace SimpleWFA {
	DWORD CheckKeyValueData(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool require_single_value);
	DWORD PassOnKeyValueData(DWORD &failed, DWORD desired_outcome, HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool require_single_value);
	DWORD CheckExplorerUserChoice(const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe);
	DWORD PassOnExplorerUserChoice(DWORD &failed, DWORD desired_outcome, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe);
	LSTATUS DeleteKeyRecursive(HKEY hKeyRoot, const wchar_t* lpSubKey);
	bool DeleteKeyValue(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName);
	bool SetValue(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool expandable_value);
}

void SimpleWFA::CreateSimpleProgID(bool local_machine, CreationDisposition creation_disp, const wchar_t* prog_id, const wchar_t* prog_path, const wchar_t* prog_desc, UINT prog_icon_res_id, const wchar_t* prog_open_params)
{
	if (!prog_id||!prog_path) return;
	
	HKEY hive=local_machine?HKEY_LOCAL_MACHINE:HKEY_CURRENT_USER;

	//If creation disposition is not UPDATE_ALWAYS then check if ProgID already exists
	if (creation_disp!=CD_UPDATE_ALWAYS&&CheckKeyValueData(hive, L"Software\\Classes\\", prog_id, NULL, NULL, NULL, false)==MEETS_CONDITION) {
		//If creation disposition is CREATE_NEW then do not create or update ProgID if it already exists
		if (creation_disp==CD_CREATE_NEW)
			return;

		//If creation disposition is CREATE_ALWAYS then ProgID is recreated anew if it already exists
		if (creation_disp==CD_CREATE_ALWAYS)
			DeleteKeyValue(hive, L"Software\\Classes\\", prog_id, NULL, NULL);
	}
	
	//We create large enough buffer that can accomodate application path (prog_path) with both DefaultIcon and Open command values
	//Application path will be incapsulated in double quotes which are also counted
	//	@="",-4294967295 - 14 characters, 4294967295 is maximum UINT
	//	@="" "%1" - 7 characters
	//	@="" PROG_OPEN_PARAMS - wcslen(prog_open_params) + 3 characters
	
	size_t pth_len=wcslen(prog_path);
	wchar_t* data_buf=new wchar_t[pth_len+(prog_open_params?std::max((size_t)14, wcslen(prog_open_params)+3):14)+1];	//+1 for terminating NULL
	wchar_t* var_buf=data_buf+pth_len+2;	//+2 for encapsulating double quotes
	
	//Copy quoted application path to buffer
	//At this point buffer won't be NULL-terminated
	data_buf[0]=L'"';
	wcscpy(data_buf+1, prog_path);	//+1 for leading double quote
	data_buf[pth_len+1]=L'"';	//+1 for leading double quote
	
	//Set command for 'open' shell verse
	if (prog_open_params) {
		wcscpy(var_buf, L" ");
		wcscat(var_buf, prog_open_params);
	} else {
		wcscpy(var_buf, L" \"%1\"");
	}
	SetValue(hive, L"Software\\Classes\\", prog_id, L"\\shell\\open\\command", DEFAULT_VALUE, data_buf, true);
	
	//Set DefaultIcon
	if (prog_icon_res_id>1) {
		swprintf(var_buf, 15, L",-%i", prog_icon_res_id);	//15 is 14 + terminating NULL
	} else if (prog_icon_res_id) {
		wcscpy(var_buf, L",0");
	} else {
		*var_buf=L'\0';
	}
	SetValue(hive, L"Software\\Classes\\", prog_id, L"\\DefaultIcon", DEFAULT_VALUE, data_buf, true);
	
	//Friendly name
	SetValue(hive, L"Software\\Classes\\", prog_id, NULL, DEFAULT_VALUE, prog_desc?prog_desc:prog_id, false);

	delete[] data_buf;
}

void SimpleWFA::RemoveProgID(bool local_machine, const wchar_t* prog_id)
{
	if (prog_id) DeleteKeyValue(local_machine?HKEY_LOCAL_MACHINE:HKEY_CURRENT_USER, L"Software\\Classes\\", prog_id, NULL, NULL);
}

void SimpleWFA::NotifyShell()
{
	SHChangeNotify(SHCNE_ASSOCCHANGED, SHCNF_IDLIST, NULL, NULL);
}

void SimpleWFA::Deassociate(bool local_machine, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe)
{
	if (!prog_id||!extension) return;
	
	if (MEETS_CONDITION&CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false))
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE);
	
	if (local_machine&&MEETS_CONDITION&CheckKeyValueData(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false))
		DeleteKeyValue(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE);
	
	if ((USER_CHOICE_MATCH|USER_CHOICE_AMBIG|USER_CHOICE_APPEXE)&CheckExplorerUserChoice(extension, prog_id, app_exe)) {
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\UserChoice", NULL);
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Progid");
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Application");
	}
	
	DeleteKeyValue(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id);
	DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id);
	DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\OpenWithProgids", prog_id);
}

void SimpleWFA::Associate(bool local_machine, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe)
{
	if (!prog_id||!extension) return;

	if (local_machine) {
		SetValue(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
		if (DATA_NOT_EQUAL&CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false))
			DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE);		
	} else {
		SetValue(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
	}
		
	if ((USER_CHOICE_OTHER|USER_CHOICE_AMBIG|USER_CHOICE_APPEXE)&CheckExplorerUserChoice(extension, prog_id, app_exe)) {
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\UserChoice", NULL);
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Progid");
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Application");
	}
	
	if ((VALUE_NOT_SINGLE|VALUE_NOT_FOUND)&CheckKeyValueData(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, true))
		DeleteKeyValue(HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, L"\\OpenWithProgids", NULL);
	
	if ((VALUE_NOT_SINGLE|VALUE_NOT_FOUND)&CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, true))
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Classes\\", extension, L"\\OpenWithProgids", NULL);
	
	if ((VALUE_NOT_SINGLE|VALUE_NOT_FOUND)&CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\OpenWithProgids", prog_id, NULL, true))
		DeleteKeyValue(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\OpenWithProgids", NULL);
}

LSTATUS SimpleWFA::DeleteKeyRecursive(HKEY hKeyRoot, const wchar_t* lpSubKey)
{
	HKEY hKey;
	LSTATUS ret_st;
	DWORD key_num, max_key_len, key_len;

	//Here we enumerate every child key and in turn pass it to this very function
	//In the end of the function (after recursion) parent key gets deleted
	//WARNING: lpSubKey should NOT end with backslash for this to work

	if ((ret_st=RegOpenKeyEx(hKeyRoot, lpSubKey, 0, KEY_READ, &hKey))==ERROR_SUCCESS) {
		if ((ret_st=RegQueryInfoKey(hKey, NULL, NULL, NULL, &key_num, &max_key_len, NULL, NULL, NULL, NULL, NULL, NULL))==ERROR_SUCCESS&&key_num) {
			size_t sub_key_len=wcslen(lpSubKey);
			wchar_t* path_buf=new wchar_t[sub_key_len+max_key_len+2];	//+2 is for backslash and NULL terminator
			wchar_t* key_buf=path_buf+sub_key_len+1;	//key_buf references the point in path_buf right after the end of lpSubKey+backslash
			
			//Copy lpSubKey to path_buf and terminate it with backslash and NULL
			wcscpy(path_buf, lpSubKey);
			path_buf[sub_key_len]=L'\\';
			path_buf[sub_key_len+1]=L'\0';
			
			//Because for each enumerated key we call DeleteKeyRecursive, each iteration number of enumerable key is reduced by one
			//And key that was returned by previous call to RegEnumKeyEx gets deleted
			//That's why we always call RegEnumKeyEx for index 0
			max_key_len++;
			do {
				key_len=max_key_len;
				if ((ret_st=RegEnumKeyEx(hKey, 0, key_buf, &key_len, NULL, NULL, NULL, NULL))==ERROR_SUCCESS)
					ret_st=DeleteKeyRecursive(hKeyRoot, path_buf);
			} while (ret_st==ERROR_SUCCESS);

			delete[] path_buf;
		}
		
		RegCloseKey(hKey);
	}
	
	if (ret_st==ERROR_NO_MORE_ITEMS||ret_st==ERROR_SUCCESS)
		return RegDeleteKey(hKeyRoot, lpSubKey);
	else
		return ret_st;
}

bool SimpleWFA::DeleteKeyValue(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName)
{
	bool is_deleted=false;
	wchar_t* new_sub_key=NULL;
	
	if (lpSubKey) {
		if (sub_key_pfix1) {
			new_sub_key=new wchar_t[wcslen(lpSubKey)+wcslen(sub_key_pfix1)+(sub_key_pfix2?wcslen(sub_key_pfix2):0)+1];
			wcscpy(new_sub_key, lpSubKey);
			wcscat(new_sub_key, sub_key_pfix1);
			if (sub_key_pfix2) wcscat(new_sub_key, sub_key_pfix2);
			lpSubKey=new_sub_key;
		}
		
		if (lpValueName) {
			if (RegOpenKeyEx(hKey, lpSubKey, 0, KEY_SET_VALUE, &hKey)==ERROR_SUCCESS) {
				is_deleted=RegDeleteValue(hKey, lpValueName)==ERROR_SUCCESS;				
				RegCloseKey(hKey);
			}
		} else {
			is_deleted=DeleteKeyRecursive(hKey, lpSubKey)==ERROR_SUCCESS;
		}
		
		if (new_sub_key) delete[] new_sub_key;
	}
	
	return is_deleted;
}

bool SimpleWFA::SetValue(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool expandable_value)
{
	bool is_set=false;
	wchar_t* new_sub_key=NULL;
	
	if (lpSubKey&&lpValueName&&lpData) {
		if (sub_key_pfix1) {
			new_sub_key=new wchar_t[wcslen(lpSubKey)+wcslen(sub_key_pfix1)+(sub_key_pfix2?wcslen(sub_key_pfix2):0)+1];
			wcscpy(new_sub_key, lpSubKey);
			wcscat(new_sub_key, sub_key_pfix1);
			if (sub_key_pfix2) wcscat(new_sub_key, sub_key_pfix2);
			lpSubKey=new_sub_key;
		}
		
		//Using RegCreateKeyEx instead of RegOpenKeyEx because key can be absent from registry
		if (RegCreateKeyEx(hKey, lpSubKey, 0, NULL, REG_OPTION_NON_VOLATILE, KEY_SET_VALUE, NULL, &hKey, NULL)==ERROR_SUCCESS) {
			is_set=RegSetValueEx(hKey, lpValueName, 0, expandable_value?REG_EXPAND_SZ:REG_SZ, (BYTE*)lpData, (wcslen(lpData)+1)*sizeof(wchar_t))==ERROR_SUCCESS;			
			RegCloseKey(hKey);
		}
		
		if (new_sub_key) delete[] new_sub_key;
	}
	
	return is_set;
}

SimpleWFA::AssocStatus SimpleWFA::CheckAssociationStatus(bool local_machine, bool check_registration, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe)
{
	if (!prog_id||!extension) return ASSOCIATED_DUNNO;

	DWORD check_failed=0;
	
	//ASSOCIATED_AFF
	
	if (local_machine) {
		if (check_registration) {
			PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", prog_id, NULL, NULL, NULL, false);
			if (app_exe) PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_LOCAL_MACHINE, L"Software\\Classes\\Applications\\", app_exe, NULL, NULL, NULL, false);
		}
		PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
		PassOnKeyValueData(check_failed, MEETS_CONDITION|NO_KEY_OR_VALUE, HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
	} else {
		if (check_registration) {
			PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_CURRENT_USER, L"Software\\Classes\\", prog_id, NULL, NULL, NULL, false);
			if (app_exe) PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_CURRENT_USER, L"Software\\Classes\\Applications\\", app_exe, NULL, NULL, NULL, false);
		}
		PassOnKeyValueData(check_failed, MEETS_CONDITION, HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
	}

	if (PassOnExplorerUserChoice(check_failed, USER_CHOICE_MATCH|USER_CHOICE_NONE, extension, prog_id, app_exe)==USER_CHOICE_NONE) {
		PassOnKeyValueData(check_failed, MEETS_CONDITION|KEY_IS_EMPTY|KEY_NOT_FOUND, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, true);
		PassOnKeyValueData(check_failed, MEETS_CONDITION|KEY_IS_EMPTY|KEY_NOT_FOUND, HKEY_CURRENT_USER, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, true);
		PassOnKeyValueData(check_failed, MEETS_CONDITION|KEY_IS_EMPTY|KEY_NOT_FOUND, HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\OpenWithProgids", prog_id, NULL, true);
	}

	if (!check_failed) return ASSOCIATED_AFF;
	
	//ASSOCIATED_NEG
	
	if (check_registration) {
		check_failed=0;
		PassOnKeyValueData(check_failed, KEY_NOT_FOUND, HKEY_CURRENT_USER, L"Software\\Classes\\", prog_id, NULL, NULL, NULL, false);
		PassOnKeyValueData(check_failed, KEY_NOT_FOUND, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", prog_id, NULL, NULL, NULL, false);
		if (app_exe) {
			PassOnKeyValueData(check_failed, KEY_NOT_FOUND, HKEY_CURRENT_USER, L"Software\\Classes\\Applications\\", app_exe, NULL, NULL, NULL, false);
			PassOnKeyValueData(check_failed, KEY_NOT_FOUND, HKEY_LOCAL_MACHINE, L"Software\\Classes\\Applications\\", app_exe, NULL, NULL, NULL, false);
		}
		if (!check_failed) return ASSOCIATED_NEG;
	}
	
	check_failed=0;
	if (PassOnExplorerUserChoice(check_failed, USER_CHOICE_NONE, extension, prog_id, app_exe)==USER_CHOICE_OTHER)
		return ASSOCIATED_NEG;
	PassOnKeyValueData(check_failed, NO_KEY_OR_VALUE, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, false);
	PassOnKeyValueData(check_failed, NO_KEY_OR_VALUE, HKEY_CURRENT_USER, L"Software\\Classes\\", extension, L"\\OpenWithProgids", prog_id, NULL, false);
	PassOnKeyValueData(check_failed, NO_KEY_OR_VALUE, HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\OpenWithProgids", prog_id, NULL, false);
	if (PassOnKeyValueData(check_failed, NO_KEY_OR_VALUE, HKEY_CURRENT_USER, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false)==DATA_NOT_EQUAL)
		return ASSOCIATED_NEG;
	PassOnKeyValueData(check_failed, NO_KEY_OR_VALUE|DATA_NOT_EQUAL, HKEY_LOCAL_MACHINE, L"Software\\Classes\\", extension, NULL, DEFAULT_VALUE, prog_id, false);
	
	//ASSOCIATED_DUNNO
	
	return check_failed?ASSOCIATED_DUNNO:ASSOCIATED_NEG;
}

DWORD SimpleWFA::PassOnExplorerUserChoice(DWORD &failed, DWORD desired_outcome, const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe)
{
	if (failed) return 0;
	
	DWORD chk_st=CheckExplorerUserChoice(extension, prog_id, app_exe);
	
	failed=~desired_outcome&chk_st;
	return chk_st;
}

DWORD SimpleWFA::CheckExplorerUserChoice(const wchar_t* extension, const wchar_t* prog_id, const wchar_t* app_exe)
{
	DWORD chk_st;
	bool pid_eq=false;
	bool app_eq=false;
	int exists=0;

	chk_st=CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, L"\\UserChoice", L"Progid", prog_id, false);
	if (chk_st==MEETS_CONDITION) {
		pid_eq=true;
		exists++;
	} else if (chk_st==DATA_NOT_EQUAL) {
		exists++;
	}
	
	if (chk_st!=SECURITY_ISSUE) {
		chk_st=CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Progid", prog_id, false);
		if (chk_st==MEETS_CONDITION) {
			pid_eq=true;
			exists++;
		} else if (chk_st==DATA_NOT_EQUAL) {
			exists++;
		}

		if (chk_st!=SECURITY_ISSUE&&exists<2) {
			chk_st=CheckKeyValueData(HKEY_CURRENT_USER, L"Software\\Microsoft\\Windows\\CurrentVersion\\Explorer\\FileExts\\", extension, NULL, L"Application", app_exe, false);
			if (chk_st==MEETS_CONDITION) {
				app_eq=true;
				exists++;
			} else if (chk_st==DATA_NOT_EQUAL) {
				exists++;
			}
		}
	}

	if (chk_st==SECURITY_ISSUE)
		return USER_CHOICE_ERROR;
	else if (exists>1)
		return USER_CHOICE_AMBIG;
	else if (app_eq&&!app_exe)
		return USER_CHOICE_APPEXE;
	else if (pid_eq||app_eq)
		return USER_CHOICE_MATCH;
	else if (exists)
		return USER_CHOICE_OTHER;
	else
		return USER_CHOICE_NONE;
}

DWORD SimpleWFA::PassOnKeyValueData(DWORD &failed, DWORD desired_outcome, HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool require_single_value)
{
	if (failed) return 0;
	
	DWORD chk_st=CheckKeyValueData(hKey, lpSubKey, sub_key_pfix1, sub_key_pfix2, lpValueName, lpData, require_single_value);
	
	failed=~desired_outcome&chk_st;
	return chk_st;
}

DWORD SimpleWFA::CheckKeyValueData(HKEY hKey, const wchar_t* lpSubKey, const wchar_t* sub_key_pfix1, const wchar_t* sub_key_pfix2, const wchar_t* lpValueName, const wchar_t* lpData, bool require_single_value)
{
	LSTATUS ret_st;
	DWORD buf_len, val_num;
	wchar_t* new_sub_key=NULL;
	DWORD chk_st=KEY_NOT_FOUND;
	
	if (lpSubKey&&sub_key_pfix1) {
		new_sub_key=new wchar_t[wcslen(lpSubKey)+wcslen(sub_key_pfix1)+(sub_key_pfix2?wcslen(sub_key_pfix2):0)+1];
		wcscpy(new_sub_key, lpSubKey);
		wcscat(new_sub_key, sub_key_pfix1);
		if (sub_key_pfix2) wcscat(new_sub_key, sub_key_pfix2);
		lpSubKey=new_sub_key;
	}

	if (!lpSubKey||(ret_st=RegOpenKeyEx(hKey, lpSubKey, 0, KEY_READ, &hKey))==ERROR_SUCCESS) {
		chk_st=VALUE_NOT_FOUND;
		
		if (lpValueName&&(ret_st=RegQueryInfoKey(hKey, NULL, NULL, NULL, NULL, NULL, NULL, &val_num, NULL, &buf_len, NULL, NULL))==ERROR_SUCCESS) {
			//If default value is not set it doesn't count towards number of key values
			//Default value with empty string is still set default value
			if (val_num) {
				if (lpData) {
					//Returned buffer length is in bytes and, because we use Unicode version of RegQueryValueEx, actual returned buffer type is wchar_t
					//Windows always stores registry string-type data in Unicode - even if actual value type is set to REG_NONE it still can be valid Unicode string
					//If using ANSI version of RegQueryValueEx, it will internally convert returned Unicode strings to ANSI ones but only for REG_SZ, REG_MULTI_SZ and REG_EXPAND_SZ types
					//If type is REG_NONE you'll get original dta that, in case of Unicode string, won't be converted to ANSI
					//If querying default value (lpValueName is NULL or empty string) and it's not set - RegQueryValueEx returns ERROR_FILE_NOT_FOUND
					//+1 is for possible missing NULL-terminator
					wchar_t* data_buf=new wchar_t[buf_len/sizeof(wchar_t)+1];
					
					if ((ret_st=RegQueryValueEx(hKey, lpValueName, NULL, NULL, (LPBYTE)data_buf, &buf_len))==ERROR_SUCCESS) {
						//Make sure that buffer is terminated
						if (!buf_len||data_buf[buf_len/sizeof(wchar_t)-1]!=L'\0') data_buf[buf_len/sizeof(wchar_t)]=L'\0';
						
						if(_wcsicmp(data_buf, lpData)) {
							chk_st=DATA_NOT_EQUAL;
							ret_st=ERROR_FILE_NOT_FOUND;
						}
					}

					delete[] data_buf;
				} else {
					ret_st=RegQueryValueEx(hKey, lpValueName, NULL, NULL, NULL, NULL);
				}

				if (ret_st==ERROR_SUCCESS&&require_single_value&&val_num>1) {
					chk_st=VALUE_NOT_SINGLE;
					ret_st=ERROR_FILE_NOT_FOUND;
				}
			} else {
				chk_st=KEY_IS_EMPTY;
				ret_st=ERROR_FILE_NOT_FOUND;
			}
		}
		
		if (lpSubKey) RegCloseKey(hKey);
	}
	
	if (new_sub_key) delete[] new_sub_key;
	
	if (ret_st==ERROR_SUCCESS)
		return MEETS_CONDITION;
	else if (ret_st==ERROR_ACCESS_DENIED)
		return SECURITY_ISSUE;
	else
		return chk_st;
}

bool SimpleWFA::CheckIfLocalMachineAvailable()
{
	HKEY reg_key;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, L"Software\\Classes", 0, KEY_WRITE, &reg_key)==ERROR_SUCCESS) {
		//It's OK, we can write to HKLM\Software\Classes
		RegCloseKey(reg_key);
		return true;
	} else {
		//HKLM is not available
		return false;
	}
}