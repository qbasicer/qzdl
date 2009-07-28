#include "ZListable.h"
#include <string>

using namespace std;

ZListable::ZListable( QListWidget * parent, int type):QListWidgetItem(parent){
	myType = type;
}

void ZListable::setName(const char* newName){
	setText(newName);
}

const char* ZListable::getName(){
	return text().toStdString().c_str();
}
