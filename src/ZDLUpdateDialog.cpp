#include "ZDLUpdateDialog.h"

#include "ZDLConfigurationManager.h"

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
	edit->setPlainText("Loading...");
	edit->setReadOnly(true);
	layout->addWidget(edit);
	layout->addWidget(createLabel("You can download the newest version at <a href=\"http://zdl.vectec.net\">zdl.vectec.net</a>", this));

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
	http = new QHttp(this);
	http->setHost("update.vectec.net", QHttp::ConnectionModeHttp);
	QString url = "/getchangelog.php?name="ZDL_PRODUCT_ID"&id=";
	connect(http, SIGNAL(requestFinished(int,bool)), this, SLOT(requestFinished(int,bool)));
	connect(http, SIGNAL(responseHeaderReceived(const QHttpResponseHeader&)), this, SLOT(responseHeaderReceived(const QHttpResponseHeader&)));
	reqid = http->get(url + QString::number(ZDL_VERSION_ID), &buffer);
	LOGDATAO() << "Starting request " << reqid << endl;
}

void ZDLUpdateDialog::responseHeaderReceived(const QHttpResponseHeader &resp){
	LOGDATAO() << "Got response header, " << resp.statusCode() << resp.reasonPhrase() << endl;
	if(resp.statusCode() != 200){
		edit->setPlainText(QString("HTTP Error: ").append(resp.reasonPhrase()));
		reqid = -1;
	}
}

void ZDLUpdateDialog::requestFinished(int id, bool error){
	LOGDATAO() << "requestFinished, id:" << id << " error:" << error << endl;
	if(reqid > id){
		LOGDATAO() << "Returning, we're supposed to ignore this one" << endl;
		return;
	}else if(reqid < 0){
		LOGDATAO() << "Somebody else has already taken care of this for us" << endl;
		return;
	}else if(id != reqid){
		LOGDATAO() << "Bad request ID. We were expecting" << reqid << endl;
		edit->setPlainText("Internal HTTP error");
		return;
	}else if(error){
		LOGDATAO() << "HTTP error " << http->errorString() << endl;
		edit->setPlainText("HTTP Error: "+http->errorString());
		return;
	}
	QByteArray ba = buffer.buffer();
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

