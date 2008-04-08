
#include "ZListable.h"
#include "ZFileListable.h"
#include <string>
#include <QFileInfo>

using namespace std;

ZFileListable::ZFileListable( QListWidget *parent, int type, QString &file):ZListable(parent, type){
	QFileInfo qfile(file);
	QString cname = qfile.fileName();
	setName(cname.toStdString().c_str());
	

}
	


