
#include "ZListable.h"
#include "ZFileListable.h"
#include <string>
#include <QFileInfo>

using namespace std;

ZFileListable::ZFileListable( QListWidget *parent, int type, const char* file):ZListable(parent, type){

	QFileInfo qfile(file);
	QString cname = qfile.fileName();
	QString list = QString("%1 [%2]").arg(cname).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;

}


ZFileListable::ZFileListable( QListWidget *parent, int type, QString &file):ZListable(parent, type){
	QFileInfo qfile(file);
	QString cname = qfile.fileName();
	QString list = QString("%1 [%2]").arg(cname).arg(file);
	setName(list.toStdString().c_str());
	fileName = file;

}

const char* ZFileListable::getFile(){
	return fileName.toStdString().c_str();
}
	


