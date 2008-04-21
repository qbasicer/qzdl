#ifndef _ZNAMEINPUT_H_
#define _ZNAMEINPUT_H_

#include <QtGui>
#include <QObject>

class ZNameInput: public QDialog{
	Q_OBJECT
	public: 
		ZNameInput(QWidget *parent);
		const char* getName();
		const char* getFile();
		void setFilter(QStringList inFilters);
	public slots:
		void browse();
	protected:
		QLineEdit *lname;
		QLineEdit *lfile;
		QStringList filters;
		QPushButton *btnBrowse;
};

#endif
