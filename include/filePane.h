#include <QtGui>
#include <QObject>
#include "ZQWidget.h"
#include "ZFileList.h"

class filePane: public ZQWidget{
Q_OBJECT
public: 
	filePane( QWidget *parent=0);
	void rebuild();
private:
	
	ZFileList *fList;	
};
