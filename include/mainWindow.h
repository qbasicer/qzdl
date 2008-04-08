#ifndef _MAINWINDOW_H_
#define _MAINWINDOW_H_

#include <QtGui>
#include <QApplication>
#include <QMainWindow>
#include "ZQWidget.h"
#include "zdlInterface.h"
#include "zSettingsPane.h"

class mainWindow: public QMainWindow{
    Q_OBJECT
public:
	mainWindow( QWidget *parent=0);
	void startRead();
	void writeConfig();

private:
	zdlInterface* intr;
	zSettingsPane* settings;
};
#endif
