#ifndef _ZDLIMPORTDIALOG_HPP_
#define _ZDLIMPORTDIALOG_HPP_

#include <QtWidgets>

class ZDLImportDialog : public QDialog {
	Q_OBJECT
	public:
		enum ImportAction{UNKNOWN, ASKLATER, IMPORTNOW, DONOTIMPORTTHIS, NEVERIMPORT};
		ZDLImportDialog(QWidget *parent);
		ImportAction getImportAction(){return importAction;}
	protected:
		ImportAction importAction;
	protected slots:
		void askLaterClicked();
		void importNowClicked();
		void doNotImportThisClicked();
		void neverImportClicked();
		
};




#endif
