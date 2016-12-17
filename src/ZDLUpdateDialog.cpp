#include "ZDLUpdateDialog.h"

#include "ZDLConfigurationManager.h"

#include <QLabel>
#include <QWidget>
#include <QDialog>
#include <QVBoxLayout>
#include <QPlainTextEdit>
#include <QFont>
#include <QDateTime>
#include <QByteArray>
#include <QString>
#include <QDialogButtonBox>

// Healper
static QLabel *createLabel(QString text, QWidget *parent){
	QLabel *ret = new QLabel(text, parent);
	ret->setWordWrap(true);
	return ret;
}

ZDLUpdateDialog::ZDLUpdateDialog(QWidget *parent):QDialog(parent){
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->addWidget(createLabel("There's an update available for "ZDL_ENGINE_NAME"!", this));
	layout->addWidget(createLabel("Here's a summary of the updates in the latest version: ", this));
	edit = new QPlainTextEdit(this);
	QFont fnt(edit->font());
	fnt.setFamily("Courier");
	fnt.setFixedPitch(true);
	edit->setFont(fnt);
	edit->setPlainText("Loading...");
	edit->setReadOnly(true);
	layout->addWidget(edit);
	QLabel *web = createLabel("You can download the newest version at <a href=\"http://zdl.vectec.net\">zdl.vectec.net</a>", this);
	web->setOpenExternalLinks(true);
	layout->addWidget(web);

	QDialogButtonBox *btnGrp = new QDialogButtonBox(this);
	QPushButton *btnOk = new QPushButton("Ok", this);
	QPushButton *btnLater = new QPushButton("Ask Later", this);
	btnGrp->addButton(btnOk, QDialogButtonBox::YesRole);
	btnGrp->addButton(btnLater, QDialogButtonBox::NoRole);
	layout->addWidget(btnGrp);
	connect(btnOk, SIGNAL(clicked()), this, SLOT(accept()));
	connect(btnLater, SIGNAL(clicked()), this, SLOT(remindLater()));	
	setSizeGripEnabled(true);
	setWindowTitle("Update available for "ZDL_ENGINE_NAME);
	reqid = 0;

    url = QUrl("update.vectec.net", QUrl::StrictMode);
    url.setPath("/getchangelog.php");
    QString uquery("name=%1&id=%2");
    uquery.arg(ZDL_PRODUCT_ID, QString.setNum(ZDL_VERSION_ID));
    url.setQuery(uquery);

    http = new QNetworkRequest(url);

    connect(reply, SIGNAL(finished()), this, SLOT(requestFinished()));
    //connect(reply, SIGNAL(responseHeaderReceived(const QHttpResponseHeader&)), this, SLOT(responseHeaderReceived(const QHttpResponseHeader&)));
	LOGDATAO() << "Starting request " << reqid << endl;
    reply = net.get(http);
}
/*
void ZDLUpdateDialog::responseHeaderReceived(const QHttpResponseHeader &resp){
	LOGDATAO() << "Got response header, " << resp.statusCode() << resp.reasonPhrase() << endl;
	if(resp.statusCode() != 200){
		edit->setPlainText(QString("HTTP Error: ").append(resp.reasonPhrase()));
		reqid = -1;
	}
}
*/
void ZDLUpdateDialog::requestFinished(){
    if(reply->error()){
        LOGDATAO() << "HTTP error " << reply->errorString() << endl;
        edit->setPlainText("HTTP Error: "+reply->errorString());
		return;
	}
    QByteArray ba = reply->readAll();
	QString data(ba);
	if(!data.isNull()){
		edit->setPlainText(data);
		LOGDATAO() << "Request completed" << endl;
	}else{
		LOGDATAO() << "Request completed, but no valid data" << endl;
	}
}

void ZDLUpdateDialog::remindLater(){
	ZDLConf *zconf = ZDLConfigurationManager::getActiveConfiguration();
	if(zconf){
		zconf->setValue("zdl.net", "lastchecked", QDateTime::currentDateTime().toString());
		zconf->deleteValue("zdl.net", "hasupdate");
	}

	reject();

}

