#ifndef _ZSPLIST_H_
#define _ZSPLIST_H_
#include "zListWidget.h"
class ZSPList : public zListWidget{
public:
	ZSPList(ZQWidget *parent);
	virtual void addButton();
	virtual void rebuild();
	virtual void newConfig();
	virtual void editButton(QListWidgetItem * item);

};

#endif

