#include <iostream>
#include <QtGui>
#include <QApplication>
#include "ZQWidget.h"
#include <QMetaObject>


ZQWidget::ZQWidget(ZQWidget *parent):QWidget(parent){
	setZParent(parent);
	//std::cout << "Using ZQWidget as parent" << std::endl;
	setContentsMargins(0,0,0,0);
}

void ZQWidget::setZParent(ZQWidget *parent){
	zparent = parent;
	connect(parent, SIGNAL( buildChildren(ZQWidget*) ), this, SLOT(notifyFromParent(ZQWidget*)));
	connect(this, SIGNAL(buildParent(ZQWidget*)), parent, SLOT(notifyFromChild(ZQWidget*)));
	connect(parent, SIGNAL( readChildren(ZQWidget*) ), this, SLOT(readFromParent(ZQWidget*)));
	connect(this, SIGNAL(readParent(ZQWidget*)), parent, SLOT(readFromChild(ZQWidget*)));
}

ZQWidget::ZQWidget(){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
	//std::cout << "Using QWidget as parent" << std::endl;
}

ZQWidget::ZQWidget(QWidget *parent):QWidget(parent){
	setContentsMargins(0,0,0,0);
	zparent = NULL;
	//std::cout << "Using QWidget as parent" << std::endl;
}

void ZQWidget::notifyFromChild(ZQWidget *origin){
	if (origin != this){
		emit buildChildren(origin);
		emit buildParent(origin);
		//fromDownstream(origin);
		rebuild();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from child (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZQWidget::notifyFromParent(ZQWidget *origin){
	if (origin != this){
		emit buildChildren(origin);
		//fromUpstream(origin);
		rebuild();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from parent (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZQWidget::readFromChild(ZQWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		emit readParent(origin);
		newConfig();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from child (of new config) (I am " << qmo->className() << ")" << std::endl;
	}
}

void ZQWidget::readFromParent(ZQWidget *origin){
	if (origin != this){
		emit readChildren(origin);
		newConfig();
		//const QMetaObject * qmo = metaObject();
		//std::cout << "Notify from parent (of new config) (I am " << qmo->className() << ")" << std::endl;
	}
}

//void ZQWidget::fromDownstream(ZQWidget *origin){
//}

//void ZQWidget::fromUpstream(ZQWidget *origin){
//}

void ZQWidget::rebuild(){
}

void ZQWidget::newConfig(){
}






