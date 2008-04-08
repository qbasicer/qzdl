#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "zListWidget.h"

class zSettingsPane: public ZQWidget{
Q_OBJECT
public: 
	zSettingsPane(QWidget *parent);
private:
	QLineEdit *alwaysArgs;
	zListWidget *iwadList;
	zListWidget *sourceList;
};
