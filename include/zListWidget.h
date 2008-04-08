#ifndef _ZLISTWIDGET_H_
#define _ZLISTWIDGET_H_

#include <QtGui>
#include <QObject>
#include <vector>
#include "ZQWidget.h"
#include "ZListable.h"

using namespace std;

class zListWidget: public ZQWidget{
Q_OBJECT
public: 
	zListWidget(ZQWidget *parent);
	virtual void insert(ZListable *item, int index);
	virtual int count();
	virtual void remove(int index);
	virtual ZListable* get(int index);
	//virtual vector<ZListable*> getList();
	//virtual void setList(vector<ZListable*> *newlist);
protected slots:
	virtual void addButton();
	virtual void removeButton();
	virtual void upButton();
	virtual void downButton();
protected:
	QPushButton *btnAdd;
	QPushButton *btnRem;
	QPushButton *btnUp;
	QPushButton *btnDn;
	QListWidget *pList;
	vector<ZListable*> list;

};
#endif
