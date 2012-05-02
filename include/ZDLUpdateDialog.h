#ifndef _ZDLUPDATEDIALOG_H_
#define _ZDLUPDATEDIALOG_H_

#include <QtGui>
#include "zdlcommon.h"

class ZDLUpdateDialog : public QDialog {
	Q_OBJECT
	public:
		ZDLUpdateDialog(QWidget *parent);
	public slots:
		void remindLater();
};


#endif


