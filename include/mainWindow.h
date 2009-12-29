#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "ZQWidget.h"
#include "zdlInterface.h"
#include "zSettingsPane.h"
#include "ZUpdater.h"

class mainWindow: public QMainWindow{
    Q_OBJECT
public:
	mainWindow( QWidget *parent=0);
	~mainWindow();
	void startRead();
	void writeConfig();
	void setUpdater(ZUpdater *zup);
	
public slots:
	void launch();
	void quit();
	void tabChange(int index);
	void manageUpdate();
	void badLaunch();

protected:
	zdlInterface* intr;
	zSettingsPane* settings;
	ZUpdater* zup;
	QString getExecutable();
	QStringList getArguments();
	int procerr;
	QAction *qact2;
};
#endif
