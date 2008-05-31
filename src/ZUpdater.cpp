#include "ZUpdater.h"
#include "configurationManager.h"

#include <iostream>
using namespace std;

ZUpdater::ZUpdater(){
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
}

ZUpdater::~ZUpdater(){
	delete http;
}

void ZUpdater::setHost(const char* host, const int port){
	this->host = host;
	this->port = port;
}

int ZUpdater::hasError(){
	return errorCode;
}

int ZUpdater::hasUpdate(){
	cout << "hasUpdate" << endl;
	return updateCode;
}

void ZUpdater::fetch(){
	cout << "fetch" << endl;
	ZDLConf *zconf = configurationManager::getActiveConfiguration();
	ZDLSection *section = zconf->getSection("zdl.net");
	if (section){
		vector <ZDLLine*> fileVctr;
		section->getRegex("^updateManager$", fileVctr);
		
		for(int i = 0; i < fileVctr.size(); i++){
			if (strcmp(fileVctr[i]->getValue(), "disabled") == 0){
				return;
			}
		}
		
	}
	if (httpGetId == 0){
		buffer.clear();
		updateCode = 0;
		http->setHost(this->host, QHttp::ConnectionModeHttp, this->port);
		QHttpRequestHeader qreq("GET", "/check.php?name=zdlsharp&id=13");
		QString ua = "qZDL ";
		ua += ZDL_VERSION_STRING;
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
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.0 Cheetah";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.1 Puma";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.2 Jaguar";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.3 Panther";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.4 Tiger";
		}else if (QSysInfo::MacintoshVersion == QSysInfo::MV_9){
			ua += "MacOSX 10.5 Leopard";
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
		ua += "; UID:";
		ua += ZDL_UID;
		ua += ")";
		
		cout << "User agent:" << ua.toStdString() << endl;
		qreq.setValue("Host", this->host);
		qreq.setValue("User-Agent", ua);
		httpGetId = http->request(qreq);
		
	}
	
}

void ZUpdater::httpRequestFinished(int requestId, bool error){
	cout << "httpRequestFinished" << endl;
	if (requestId != httpGetId)
		return;
	if (error){
		cout << "error" << endl;
	}
	QString str(buffer);
	cout << "Buffer " << str.toStdString() << endl;
	if (str == "MISMATCH"){
		cout << "There is an update" << endl;
		updateCode = 1;
		errorCode = 0;
	}else if (str == "ERROR-SYNTAX"){
		updateCode = 0;
		errorCode = 1;
	}else if (str == "ERROR-NOID"){
		updateCode = 0;
		errorCode = 2;
	}else if (str == "OKAY"){
		updateCode = 0;
		errorCode = 0;
	}else{
		updateCode = 0;
	}
	emit updateReady();
}

void ZUpdater::readyRead ( const QHttpResponseHeader & resp ){
	cout << "readyRead" << endl;
	QByteArray inBytes = http->readAll();
	buffer.append(inBytes);
}

void ZUpdater::readResponseHeader(const QHttpResponseHeader &responseHeader){
	cout << "readResponseHeader" << endl;
	errorCode = responseHeader.statusCode();
	cout << "Error code: " << errorCode << endl;
	
}
