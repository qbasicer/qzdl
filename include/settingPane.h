#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class settingPane: public ZQWidget{
Q_OBJECT
public: 
	settingPane( ZQWidget *parent=0);
	virtual void rebuild();
	virtual void newConfig();
protected:
	QComboBox *diffList;
	QComboBox *sourceList;
	QListWidget *IWADList;
	
};
