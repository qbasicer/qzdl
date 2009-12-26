#ifndef _ZINFOBAR_H_
#define _ZINFOBAR_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class ZInfoBar : public ZQWidget{
Q_OBJECT
	public: 
		ZInfoBar( ZQWidget *parent=0);
		void setMessage(const char* messgae, int icon);
	protected:
		QLabel *txtlbl;
		QLabel *lbl;
		QColor warning;
		QColor info;
		QColor normal;
	protected slots:
		virtual void hidebar();
		virtual void more();
	signals:
		void moreclicked();
};

#endif

