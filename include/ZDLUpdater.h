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
 
#ifndef _ZUPDATER_H_
#define _ZUPDATER_H_

#include <QtCore>
#include <QObject>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#define UPDATE_NOUPDATE		0
#define UPDATE_MISMATCH		1
#define UPDATE_ENDOFLIFE	2

class ZDLUpdater: public QObject{
Q_OBJECT
	public:
		ZDLUpdater();
		~ZDLUpdater();
	void setHost(const char* host, const int port);
	void fetch();
	int hasUpdate();
	int hasError();
	void fetch(int doAnyways);
	
	private:
		
		QString host;
		int errorCode;
		int port;
        int updateCode;
        QUrl url;
        QNetworkRequest* http;
		QByteArray buffer;
        QNetworkAccessManager net;
        QNetworkReply* reply;
	protected slots:
        // TODO: Replace QHttp stuff
        void httpRequestFinished();
        void readyRead ();
        //void readResponseHeader(const QHttpResponseHeader &responseHeader);
		void updatesDisabledInfobar();
		void updatesOldSystem();
	signals:
		void updateReady();
		
};

#endif
