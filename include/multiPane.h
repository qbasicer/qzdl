#ifndef _MULTIPANE_H_
#define _MULTIPANE_H_

#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class multiPane: public ZQWidget{
Q_OBJECT
public: 
	multiPane( ZQWidget *parent=0);
};

#endif

