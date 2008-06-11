#include <QMetaObject>
#ifndef _ZQSPLIT_H_
#define _ZQSPLIT_H_
#include <QObject>
#include <QtGui>
#include "ZQWidget.h"


class ZQSplitter: public virtual ZQWidget{
Q_OBJECT
	public:
		ZQSplitter(ZQWidget *parent);
		ZQSplitter(QWidget *parent);
		virtual void addChild(QWidget *child);
		virtual void addChild(ZQWidget *child);
		virtual QSplitter* getSplit();
	protected:
		QSplitter *split;
		QVBoxLayout *box;
};
#endif

