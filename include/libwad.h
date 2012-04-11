#ifndef _LIBWAD_H_
#define _LIBWAD_H_

class WadLump;

class DoomWad {
	public:
		enum WadType {TYPE_IWAD, TYPE_PWAD};
		enum AddBehaviour {REWRITE_WAD, APPEND_FIXUP};
		DoomWad(QString file);
		DoomWad(QIODevice *dev);
		bool open();
		int lumps();
		WadLump *getLump(int index);
		WadLump *getLumpByName(QString name);
		bool addLumps(QList<WadLump*> lumps, DoomWad::AddBehaviour behaviour = DoomWad::APPEND_FIXUP);
		QStringList getLumpNames();
	protected:
		int numLumps;
		int directoryStart;
		QIODevice *dev;
		QStringList lumpnames;
};

// This certainly won't compile for now
class WadLump : public QIODevice{
	public:
		QString getName();
		int getSize();
	protected:
		int lumpStart;
		DoomWad *parent;
};

#endif
