#ifndef _ZDL_NULL_DEVICE_H_
#define _ZDL_NULL_DEVICE_H_

#include <QtCore>

class ZDLNullDevice : public QIODevice {
public:
	virtual qint64 readData(char* data, qint64 len){
		Q_UNUSED(data);
		Q_UNUSED(len);
		return 0;
	}

	virtual qint64 writeData(const char* data, qint64 len){
		Q_UNUSED(data);
		Q_UNUSED(len);
		return 0;
	}
};



#endif
