#ifndef _ZDLUPDATEDIALOG_H_
#define _ZDLUPDATEDIALOG_H_

#include <QDialog>
#include <QPlainTextEdit>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include "zdlcommon.h"

class ZDLUpdateDialog : public QDialog {
	Q_OBJECT
	public:
		ZDLUpdateDialog(QWidget *parent);
	public slots:
		void remindLater();
	protected slots:
		void requestFinished(int id, bool error);
        //void responseHeaderReceived(const QHttpResponseHeader &resp);
	protected:
        //QHttp *http;
        QUrl url;
        QNetworkRequest *http;
        QNetworkReply *reply;
        QNetworkAccessManager net;
		QBuffer buffer;
		QPlainTextEdit *edit;
};


#endif


