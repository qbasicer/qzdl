#include <QtGui>
#include <QObject>
#include "ZQWidget.h"

class buttonPane: public ZQWidget{
Q_OBJECT
public: 
	buttonPane( ZQWidget *parent=0);
	virtual void newConfig();
private slots:
	void sendSignals();
	void mclick();
	void launch();
	void ampclick();
	void saveConfigFile();
	void loadConfigFile();
	void aboutClick();
private:
	QPushButton *btnEpr;
	QPushButton *btnZDL;
};
