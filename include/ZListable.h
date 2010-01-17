#ifndef _ZLISTABLE_H_
#define _ZLISTABLE_H_

#include <QListWidgetItem>
#include <QObject>

class ZListable: public QListWidgetItem{
public:
	virtual const char* getName();
	ZListable( QListWidget * parent, int type);
	virtual void setName(const char* inName);
	virtual void setName(QString name);
protected:
	int myType;
};

#endif

