#ifndef _ZDLFILEASSOCIATIONS_H_
#define _ZDLFILEASSOCIATIONS_H_

#include <QtGui>

class ZDLFileAssociations : public QDialog {
	Q_OBJECT
	public:
		ZDLFileAssociations(QWidget *parent);
	protected slots:
		void applyChecks();
	protected:
		QCheckBox *chkZDL;
		QCheckBox *chkWAD;
		QCheckBox *chkPK3;
		QCheckBox *chkZIP;
		QCheckBox *chkPATCH;
};

#endif
