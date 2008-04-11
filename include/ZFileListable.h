#ifndef _ZFILELISTABLE_H_
#define _ZFILELISTABLE_H_

#include "ZListable.h"

class ZFileListable : public ZListable{
public:
	ZFileListable( QListWidget * parent, int type, QString &qfile);
	const char* getFile();

	private:
		QString fileName;
};

#endif
