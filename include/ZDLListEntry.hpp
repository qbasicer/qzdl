#ifndef _ZLISTENTRY_HPP_
#define _ZLISTENTRY_HPP_

#include <QtCore>
#include "ZQWidget.h"

class ZDLListEntry {
	public:
		/* Originator allows us to see who created this entry */
		/* Type is something descriptive but FQDN-like */
		/* ie net.vectec.qzdl.lists.iwad */
		ZDLListEntry(QString originator, QString type);
		ZDLListEntry(ZQWidget originator, QString type);
		
		/* Be stupidly flexible and allow any data */
		/* to be added without restricting */
		/* how much to accept */
		void addData(QString key, QVariant newData);
		void removeData(QString key);
		QVariant getData(QString key);

		QString getType(){return type;}
		QString getOriginator(){return originator;}
	protected:
		QString originator;
		QString type;
		QHash<QString, QVariant> data;
};

#endif
