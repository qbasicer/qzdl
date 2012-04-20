#ifndef _ZDLMAPFILE_HPP_
#define _ZDLMAPFILE_HPP_

#include <QtCore>

class ZDLMapFile{
	public:
		static ZDLMapFile *getMapFile(QString file);
		virtual QStringList getMapNames() = 0;
		virtual bool isCompressed() = 0;
	protected:
};


#endif



