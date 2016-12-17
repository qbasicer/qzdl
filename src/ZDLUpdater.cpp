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
	/*
    connect(net, SIGNAL(responseHeaderReceived(const QHttpResponseHeader &)),
			this, SLOT(readResponseHeader(const QHttpResponseHeader &)));
    connect(net, SIGNAL(requestFinished(int, bool)),
            this, SLOT(httpRequestFinished(int, bool)));
	*/
    connect(reply, SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(reply, SIGNAL(finished()), this, SLOT(httpRequestFinished()));
	host = "update.vectec.net";
	errorCode = 0;
	port = 80;
    url = QUrl(QString("http://%1:%2/").arg(host, QString().setNum(port)), QUrl::StrictMode);
    http = new QNetworkRequest(url);
    updateCode = 0;
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
    url.setHost(QString("http://%1:%2/").arg(host, QString().setNum(port)));
    http->setUrl(url);
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
	// Check if updates are disabled only if updates aren't forced
	if (section && doAnyways == 0){
		QVector<ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);

		for(int i = 0; i < fileVctr.size(); i++){
			if (fileVctr[i]->getValue().compare("disabled") == 0){
				return;
				//ZDLConfigurationManager::setInfobarMessage("Updates are disabled.",1);
				//ZDLInfoBar *bar = (ZDLInfoBar*)ZDLConfigurationManager::getInfobar();
				//connect(bar,SIGNAL(moreclicked()),this,SLOT(updatesDisabledInfobar()));
				//if(doAnyways == 0){
				//	return;
				//}
			}
		}

	}

	// If we haven't specified the version we last checked the version for, force check
	if(!zconf->hasValue("zdl.net", "hasupdateversion")){
		doAnyways = 1;
		LOGDATAO() << "No hasupdateversion, forcing update" << endl;
	}else{
		int ok = 0;
		QString rc = zconf->getValue("zdl.net","hasupdateversion", &ok);
		if(rc != QString::number(ZDL_VERSION_ID)){
			LOGDATAO() << "Mismatched hasupdateversion, forcing update" << endl;
			// Force check if version numbers mismatch (we just updated most likely)
			doAnyways = 1;
		}
	}

	if(zconf && zconf->hasValue("zdl.net", "lastchecked") && doAnyways == 0){
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
					LOGDATAO() << "It's not been 24h since the last update check" << endl;
					return;
				}
			}
		}
	}

    if (reply == NULL || !reply->isRunning()) {
		buffer.clear();
		updateCode = 0;
        url.setHost(host, QUrl::StrictMode);
        url.setPort(port);

        QString upath("/check.php");
        QString uquery("name=%1&id=%2");
        uquery.arg(ZDL_PRODUCT_ID, QString().setNum(ZDL_VERSION_ID));
        url.setPath(upath, QUrl::StrictMode);
        url.setQuery(uquery, QUrl::StrictMode);

        // Construct UA string
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
            }else if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS8){
                ua += "Windows 8";
            }else if (QSysInfo::WindowsVersion == QSysInfo::WV_WINDOWS10){
                ua += "Windows 10";
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
#elif Q_WS_MAC
		if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
            ua += "Mac OS 9";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_0){
            ua += "Mac OS X 10.0 Cheetah";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_1){
            ua += "Mac OS X 10.1 Puma";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_2){
            ua += "Mac OS X 10.2 Jaguar";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_3){
            ua += "Mac OS X 10.3 Panther";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_4){
            ua += "Mac OS X 10.4 Tiger";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_5){
            ua += "Mac OS X 10.5 Leopard";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_6){
            ua += "Mac OS X 10.6 Snow Leopard";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_7){
            ua += "Mac OS X 10.7 Lion";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_8){
            ua += "Mac OS X 10.8 Mountain Lion";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_9){
            ua += "Mac OS X 10.9 Mavericks";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_10){
            ua += "Mac OS X 10.10 Yosemite";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_11){
            ua += "Mac OS X 10.11 El Capitan";
        }else if (QSysInfo::MacintoshVersion == QSysInfo::MV_10_12){
            ua += "macOS 10.12 Sierra";
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

        // Find out what Linux distro the user is using.


        bool lsbError = false;
        QString* distro;
        QProcess lsb;
        lsb.start("/usr/bin/lsb_release", QStringList() << "-d");
        connect(lsb, static_cast<void(QProcess::*)(int, QProcess::ExitStatus)>(&QProcess::finished),
                [&](int exitCode, QProcess::ExitStatus exitStatus){
            if (exitCode == 0 && exitStatus == QProcess::NormalExit) {
                QByteArray lsbout = lsb.readAllStandardOutput();
                lsbout.remove(0, QString("Description: ").length());
                distro = new QString(lsbout);
            } else {
                lsbError = true;
            }
        });
        QObject::connect(&lsb, &QProcess::errorOccurred(QProcess::ProcessError), [&lsbError](QProcess::ProcessError error){
            lsbError = true;
        });
        lsb.waitForFinished(-1);

        if (!lsbError) {
            ua += "; ";
            ua += distro;
            delete distro;
        }
#endif

#if defined(USE_UID)
		LOGDATAO() << "UID: " << ZDL_UID << endl;
		ua += "; UID:";
		ua += ZDL_UID;
#endif
		ua += ")";
		LOGDATAO() << "User-Agent: " << ua << endl;
		//cout << "User agent:" << ua.toStdString() << endl;
        //qreq.setValue("Host", this->host);
        http->setHeader(QNetworkRequest::UserAgentHeader, ua);
        reply = net.get(*http);
        LOGDATAO() << "Request started" << endl;
    }else{
		LOGDATAO() << "Can't start multiple requests" << endl;
	}

}

void ZDLUpdater::httpRequestFinished(){
    LOGDATAO() << "Finished request.";
    /*
	if (requestId != httpGetId){
        LOGDATAO() << "Internal HTTP error, Buffer was: " << buffer << endl;
		return;
	}
    */
    if (reply->error()){
        LOGDATAO() << "Error! Buffer was: " << buffer << endl;
		return;
	}
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	zconf->setValue("zdl.net", "lastchecked", QDateTime::currentDateTime().toString());
	QString str(buffer);
	LOGDATAO() << "Got: " << str << endl;
	if (str == "MISMATCH"){
		LOGDATAO() << "There is an update available" << endl;
		updateCode = 1;
		errorCode = 0;
		if(zconf){
			// Update with current version information
			zconf->setValue("zdl.net", "hasupdate", "1");
			zconf->setValue("zdl.net", "hasupdateversion", ZDL_VERSION_ID);
		}
	}else{
		// Server didn't understand us
		if (str == "ERROR-SYNTAX"){
			LOGDATAO() << "Update syntax error" << endl;
			updateCode = 0;
			errorCode = 1;
		// No ID matching our request
		}else if (str == "ERROR-NOID"){
			LOGDATAO() << "No such ID" << endl;
			updateCode = 0;
			errorCode = 2;
		// Current version matches our version, no update
		}else if (str == "OKAY"){
			LOGDATAO() << "No update available" << endl;
			updateCode = 0;
			errorCode = 0;
			zconf->setValue("zdl.net", "hasupdateversion", ZDL_VERSION_ID);
			zconf->setValue("zdl.net", "hasupdate", "0");
		}else if (str == "ENDOFLIFE"){
			LOGDATAO() << "End of life" << endl;
			updateCode = 2;
			errorCode = 0;
		//Unknown error
		}else{
			LOGDATAO() << "Unexpected content" << endl;
			updateCode = 0;
			errorCode = 3;
		}
		if(zconf){
			zconf->deleteValue("zdl.net", "hasupdate");
		}
	}
    LOGDATAO() << "updateCode:" << updateCode << " errorCode:" << errorCode << endl;
	emit updateReady();
}

void ZDLUpdater::readyRead () {
    LOGDATAO() << "ReadyRead (" << reply->bytesAvailable() << " bytes to read)" << endl;
    errorCode = reply->error();
    if (errorCode == QNetworkReply::NoError) {
        buffer.append(reply->readAll());
    } else {
        ZDLConfigurationManager::setInfobarMessage("There was an unexpected HTTP response code checking for updates",1);
    }
}
