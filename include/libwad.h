#ifndef _LIBWAD_H_
#define _LIBWAD_H_

#include <QtCore>

class WadLump;

class DoomWad : public ZDLMapFile{
	public:
		enum WadType {TYPE_IWAD, TYPE_PWAD};
		enum AddBehaviour {REWRITE_WAD, APPEND_FIXUP};
		DoomWad(QString file){}
		DoomWad(QIODevice *dev){}
		bool open(){return FALSE;}
		int lumps(){return 0;}
		WadLump *getLump(int index){return NULL;}
		WadLump *getLumpByName(QString name){return NULL;}
		bool addLumps(QList<WadLump*> lumps, DoomWad::AddBehaviour behaviour = DoomWad::APPEND_FIXUP){return false;}
		QStringList getLumpNames(){return QStringList();}
		virtual QStringList getMapNames(){return QStringList();}
		virtual bool isCompressed(){return false;}
	protected:
		int numLumps;
		int directoryStart;
		QIODevice *dev;
		QStringList lumpnames;
};

// This certainly won't compile for now
class WadLump : public QIODevice{
	public:
		QString getName(){return QString("");}
		int getSize(){return 0;}
	protected:
		int lumpStart;
		DoomWad *parent;
};

#endif
