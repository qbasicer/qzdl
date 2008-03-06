#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class filePane: public ZQWidget{
Q_OBJECT
public: 
	filePane( ZQWidget *parent=0);
	void rebuild();
};
