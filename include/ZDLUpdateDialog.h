#ifndef _ZDLUPDATEDIALOG_H_
#define _ZDLUPDATEDIALOG_H_

#include <QtGui>
#include <QHttp>
#include "zdlcommon.h"

class ZDLUpdateDialog : public QDialog {
	Q_OBJECT
	public:
		ZDLUpdateDialog(QWidget *parent);
	public slots:
		void remindLater();
	protected slots:
		void requestFinished(int id, bool error);
		void responseHeaderReceived(const QHttpResponseHeader &resp);
	protected:
		QHttp *http;
		int reqid;
		QBuffer buffer;
		QPlainTextEdit *edit;
};


#endif


