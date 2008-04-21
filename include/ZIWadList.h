#ifndef _ZIWADLIST_H_
#define _ZIWADLIST_H_
#include "zListWidget.h"
class ZIWadList : public zListWidget{
public:
	ZIWadList(ZQWidget *parent);
	virtual void addButton();
	virtual void rebuild();
	virtual void newConfig();

};

#endif

