#include <QtCore>
#include <QObject>
#include <QHttp>

class ZUpdater: public QObject{
Q_OBJECT
	public:
		ZUpdater();
		~ZUpdater();
	void setHost(const char* host, const int port);
	void fetch();
	int hasUpdate();
	int hasError();
	
	private:
		
		QString host;
		int errorCode;
		int port;
		int updateCode;
		int httpGetId;
		QHttp *http;
		QByteArray buffer;
	protected slots:
		void httpRequestFinished(int requestId, bool error);
		void readyRead (const QHttpResponseHeader &resp);
		void readResponseHeader(const QHttpResponseHeader &responseHeader);
	signals:
		void updateReady();
		
};
