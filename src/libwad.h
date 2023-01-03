#ifndef _LIBWAD_H_
#define _LIBWAD_H_

class DoomWad;

#include <QtCore>
#include "ZDLMapFile.h"

class WadLump;

class DoomWad : public ZDLMapFile {
	public:
		enum WadType {TYPE_IWAD, TYPE_PWAD};
		enum AddBehaviour {REWRITE_WAD, APPEND_FIXUP};
		DoomWad(QString file);
		DoomWad(QIODevice *dev);
		virtual ~DoomWad();
		virtual bool open();
		int lumps();
		WadLump *getLump(int index);
		WadLump *getLumpByName(QString name);
		bool addLumps(QList<WadLump*> lumps, DoomWad::AddBehaviour behaviour = DoomWad::APPEND_FIXUP);
		QStringList getLumpNames();
		virtual QStringList getMapNames();
		virtual bool isCompressed();
	protected:
		int numLumps;
		int directoryStart;
		QIODevice *dev;
		QStringList levelnames;
		QVector<WadLump*> wadLumps;
		bool shouldClose;
};

// This certainly won't compile for now
class WadLump : public QIODevice{
	public:
		WadLump(int start, int size, QString name, DoomWad *par);
		QString getName();
		int getSize();
		virtual qint64 readData(char* buf, qint64 len){return 0;}
		virtual qint64 writeData(const char* buf, qint64 len){return 0;}
	protected:
		int lumpStart;
		int lumpSize;
		QString lumpName;
		DoomWad *parent;
};

#endif
