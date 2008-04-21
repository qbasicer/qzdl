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
private:
	QPushButton *btnEpr;
};
