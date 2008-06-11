#include <QMetaObject>
#ifndef _ZQWIDGET_H_
#define _ZQWIDGET_H_
#include <QObject>
#include <QWidget>


class ZQWidget: public QWidget{
Q_OBJECT
public: 
	ZQWidget(ZQWidget *parent);
	ZQWidget(QWidget *parent);
	ZQWidget();
	virtual void setZParent(ZQWidget *parent);
	virtual void rebuild();
	virtual void newConfig();
signals:
	void buildChildren(ZQWidget*);
	void buildParent(ZQWidget*);
	void readChildren(ZQWidget*);
	void readParent(ZQWidget*);
public slots:
	virtual void notifyFromChild(ZQWidget*);
	virtual void notifyFromParent(ZQWidget*);
	virtual void readFromChild(ZQWidget*);
	virtual void readFromParent(ZQWidget*);
protected:
	virtual void fromUpstream(ZQWidget *origin);
	virtual void fromDownstream(ZQWidget *origin);

private:
	ZQWidget *zparent;
};
#endif

