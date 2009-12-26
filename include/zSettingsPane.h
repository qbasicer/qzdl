#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "ZSPList.h"
#include "ZIWadList.h"

class zSettingsPane: public ZQWidget{
Q_OBJECT
public: 
	zSettingsPane(QWidget *parent);
	virtual void rebuild();
	virtual void newConfig();
	void startRead();
	void writeConfig();

private:
	QLineEdit *alwaysArgs;
	ZIWadList *iwadList;
	ZSPList *sourceList;
	QCheckBox *updater;
	
};
