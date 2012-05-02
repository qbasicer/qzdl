/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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

#include <QMessageBox>
#include "ZDLUpdater.h"
#include "ZDLConfigurationManager.h"
#include "ZDLInfoBar.h"
#include <iostream>
using namespace std;
extern QString versionString;
ZDLUpdater::ZDLUpdater(){
	LOGDATAO() << "New updater" << endl;
	http = new QHttp(this);
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
			this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
	connect(http, SIGNAL(requestFinished(int, bool)),
			this, SLOT(httpRequestFinished(int, bool)));
	connect(http, SIGNAL(readyRead(const QHttpResponseHeader &)),
			this, SLOT(readyRead(const QHttpResponseHeader &)));
	host = "update.vectec.net";
	errorCode = 0;
	port = 80;
	updateCode = 0;
	httpGetId = 0;
	LOGDATAO() << "Done" << endl;
}

ZDLUpdater::~ZDLUpdater(){
	LOGDATAO() << "Destroying updater" << endl;
	delete http;
}

void ZDLUpdater::setHost(const char* host, const int port){
	LOGDATAO() << "setHost " << host << port << endl;
	this->host = host;
	this->port = port;
}

int ZDLUpdater::hasError(){
	return errorCode;
}

int ZDLUpdater::hasUpdate(){
	//cout << "hasUpdate" << endl;
	return updateCode;
}

void ZDLUpdater::updatesDisabledInfobar(){
	QMessageBox::warning(NULL,"Updates Disabled", "You have disabled checking for updates.  This means that you will no longer get notification of new releases.\n\nIf you did not disable the updates yourself, then " ZDL_ENGINE_NAME " has automatically disabled updates silently if there were connectivity issues with the update server.\n\nTo re-enable updates, please go to the settings tab in the main interface.",QMessageBox::Ok,QMessageBox::Ok);
}

void ZDLUpdater::updatesOldSystem(){
	QMessageBox::warning(NULL,"Old Update System", "Thank you for trying " ZDL_ENGINE_NAME "!!\n\nCurrently updates are pushed by a rather limited script located on our server.  Unfortunately, it doesn't quite have the ability to check to see if your version is the most current version.  You most likely checked out our version from SVN from our sourceforge website.  We only bump the version number on major releases, so we encourage you to manually check for updates on our sourceforge project page.\n\nhttp://sf.net/projects/ZDLSharp",QMessageBox::Ok,QMessageBox::Ok);
}

void ZDLUpdater::fetch(){
	fetch(0);
}

void ZDLUpdater::fetch(int doAnyways){
	LOGDATAO() << "fetching updates, doAnyways: " << doAnyways << endl;
	Q_UNUSED(doAnyways);
	//cout << "fetch" << endl;
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();

	ZDLSection *section = zconf->getSection("zdl.net");
	if (section){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			if (fileVctr[i]->getValue().compare("disabled") == 0){
				//ZDLConfigurationManager::setInfobarMessage("Updates are disabled.",1);
				//ZDLInfoBar *bar = (ZDLInfoBar*)ZDLConfigurationManager::getInfobar();
				//connect(bar,SIGNAL(moreclicked()),this,SLOT(updatesDisabledInfobar()));
				//if(doAnyways == 0){
				//	return;
				//}
			}
		}

	}

	if(zconf && zconf->hasValue("zdl.net", "hasupdate")){
		int ok = 0;
		QString rc = zconf->getValue("zdl.net", "hasupdate", &ok);
		if(!rc.isNull()){
			if(rc == "1"){
			}
		}
	}

	if(zconf && zconf->hasValue("zdl.net", "lastchecked")){
		int ok = 0;
		QString rc = zconf->getValue("zdl.net", "lastchecked", &ok);
		if(!rc.isNull()){
			QDateTime lc = QDateTime::fromString(rc);
			if(!lc.isNull() && lc.isValid()){
				QDateTime now = QDateTime::currentDateTime();
				lc = lc.addDays(1);
				if(lc > now){
					//It's been less than 24h since the last update
					if(zconf && zconf->hasValue("zdl.net", "hasupdate")){
						int ok = 0;
						QString rc = zconf->getValue("zdl.net", "hasupdate", &ok);
						if(!rc.isNull()){
							if(rc == "1"){
								//If we already have an update, tell the user
								updateCode = 1;
								emit updateReady();
								return;
							}
						}
					}
					return;
				}
			}
		}
	}

	if (httpGetId == 0){
		buffer.clear();
		updateCode = 0;
		http->setHost(this->host, QHttp::ConnectionModeHttp, this->port);
#ifdef Q_WS_WIN
		QString url = "/check.php?name=qzdl-win32-beta&id=";
#else
		QString url = "/check.php?name=qzdl-git&id=";
#endif
		url += QString::number(ZDL_VERSION_ID);
		QHttpRequestHeader qreq("GET", url);
		QString ua = QString(ZDL_ENGINE_NAME) + QString(" ") + versionString;
		ua += " (";
#ifdef Q_WS_WIN
		if (QSysInfo::WindowsVersion & QSysInfo::WV_DOS_based){
			ua += "Win32; ";
			if (QSysInfo::WindowsVersion == QSysInfo::WV_32s){
				ua += "Windows 3.1";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_95){
				ua += "Windows 95";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_98){
				ua += "Windows 98";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_Me){
				ua += "Windows ME";
			}else {
				ua += "Windows Unknown";
			}
		}else if (QSysInfo::WindowsVersion & QSysInfo::WV_NT_based){
			ua += "WinNT; ";
			if (QSysInfo::WindowsVersion == QSysInfo::WV_NT){
				ua += "Windows NT";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_2000){
				ua += "Windows 2000";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_XP){
				ua += "Windows XP";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_2003){
				ua += "Windows Server 2003";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_VISTA){
				ua += "Windows Vista";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS7){
				ua += "Windows 7";
			}else{
				ua += "Windows Unknown";
			}

		}else if (QSysInfo::WindowsVersion & QSysInfo::WV_CE_based){
			if (QSysInfo::WindowsVersion == QSysInfo::WV_CE){
				ua += "Windows CE";
			}else if (QSysInfo::WindowsVersion == QSysInfo::WV_CENET){
				ua += "Windows CS .NET";
			}else{
				ua += "Windows Unknown";
			}

			ua += "WinCE; ";
		}else{
			ua += "WinUnknown";
		}
#else
#ifdef Q_WS_MAC 
		if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOS9";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_0){
			ua += "MacOSX 10.0 Cheetah";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_1){
			ua += "MacOSX 10.1 Puma";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_2){
			ua += "MacOSX 10.2 Jaguar";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_3){
			ua += "MacOSX 10.3 Panther";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_4){
			ua += "MacOSX 10.4 Tiger";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_5){
			ua += "MacOSX 10.5 Leopard";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_SNOWLEOPARD){
			ua += "MaxOSX 10.6 Snow Leopard";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_Unknown){
			ua += "Mac Unknown";
		}else{
			ua += "Mac Unknown";
		}
#else
		ua += "Linux ";
		QFile kernel("/proc/sys/kernel/osrelease");
		if (kernel.exists() && kernel.open(QIODevice::ReadOnly)){
			QString lkv = kernel.readLine();
			ua += lkv.section("\n", 0, 0);
		}else{
			ua += "Unknown";
		}

#endif
#endif
#if defined(USE_UID)
		LOGDATAO() << "UID: " << ZDL_UID << endl;
		ua += "; UID:";
		ua += ZDL_UID;
#endif
		ua += ")";
		LOGDATAO() << "User-Agent: " << ua << endl;
		//cout << "User agent:" << ua.toStdString() << endl;
		qreq.setValue("Host", this->host);
		qreq.setValue("User-Agent", ua);
		httpGetId = http->request(qreq);
		LOGDATAO() << "Request " << httpGetId << " started" << endl;
	}else{
		LOGDATAO() << "Can't start multiple requests" << endl;
	}

}

void ZDLUpdater::httpRequestFinished(int requestId, bool error){
	LOGDATAO() << "Finished request " << requestId << endl;
	if (requestId != httpGetId){
		LOGDATAO() << "Internal HTTP error, reqId:" << requestId << " getId:" << httpGetId << " Buffer was: " << buffer << endl;
		return;
	}
	if (error){
		LOGDATAO() << "Error!" << endl;
		return;
	}
	QString str(buffer);
	LOGDATAO() << "Got: " << str << endl;
	if (str == "MISMATCH"){
		LOGDATAO() << "There is an update available" << endl;
		updateCode = 1;
		errorCode = 0;
		if(zconf){
			zconf->setValue("zdl.net", "hasupdate", "1");
		}
	}else{
		if (str == "ERROR-SYNTAX"){
			LOGDATAO() << "Update syntax error" << endl;
			updateCode = 0;
			errorCode = 1;
		}else if (str == "ERROR-NOID"){
			LOGDATAO() << "No such ID" << endl;
			updateCode = 0;
			errorCode = 2;
		}else if (str == "OKAY"){
			LOGDATAO() << "No update available" << endl;
			updateCode = 0;
			errorCode = 0;
		}else{
			LOGDATAO() << "Unexpected content" << endl;
			updateCode = 0;
			errorCode = 3;
		}
		if(zconf){
			zconf->deleteValue("zdl.net", "hasupdate");
		}
	}
	httpGetId = 0;
	LOGDATAO() << "Reset httpGetId, updateCode:" << updateCode << " errorCode:" << errorCode << endl;
	emit updateReady();
}

void ZDLUpdater::readyRead ( const QHttpResponseHeader & resp ){
	LOGDATAO() << "ReadyRead" << endl;
	if(resp.statusCode() == 200){
		//cout << "readyRead: " << resp.reasonPhrase().toStdString() << endl;
		QByteArray inBytes = http->readAll();
		buffer.append(inBytes);
	}
}

void ZDLUpdater::readResponseHeader(const QHttpResponseHeader &responseHeader){
	//cout << "readResponseHeader" << endl;
	errorCode = responseHeader.statusCode();
	LOGDATAO() << "HTTP Header code: " << errorCode << endl;
	//cout << "Error code: " << errorCode << endl;
	if(errorCode != 200){
		ZDLConfigurationManager::setInfobarMessage("There was an unexpected HTTP response code checking for updates",1);	
	}

}
