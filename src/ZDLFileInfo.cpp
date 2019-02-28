/*
 * This file is part of qZDL
 * Copyright (C) 2018  Lcferrum
 * 
 * qZDL is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QFile>
#include <QCryptographicHash>
#include <algorithm>
#include <cstring>
#include "ZDLFileInfo.h"
#include "ZDLMapFile.h"

struct IwadHash {
	char md5[16];
	const char* name;
};

#define INT_TO_BYTES(i) ((unsigned int)i>>24)&0xFF, ((unsigned int)i>>16)&0xFF, ((unsigned int)i>>8)&0xFF, (unsigned int)i&0xFF
#define DEFINE_IWAD(n, d1, d2, d3, d4) {{INT_TO_BYTES(d1), INT_TO_BYTES(d2), INT_TO_BYTES(d3), INT_TO_BYTES(d4)}, n}

//This is sorted list of IWAD hashes
//It includes only versioned IWADs
//Original unsorted list, that is easier to manage, is provided in comments at the bottom of this source file
const IwadHash sorted_iwad_hashes[]={
	DEFINE_IWAD("Heretic Shareware v1.0", 0x023b5217, 0x5d2f260c, 0x3bdc5528, 0xdf5d0a8c),
	DEFINE_IWAD("Doom Press Release Beta", 0x049e32f1, 0x8d9c9529, 0x630366cf, 0xc72726ea),
	DEFINE_IWAD("Strife: Veteran Edition v1.0", 0x06a8f99b, 0x9b756ac9, 0x08917c38, 0x68b8e3bc),
	DEFINE_IWAD("Strife Voices WAD", 0x082234d6, 0xa3f70864, 0x24856478, 0xb5aa9e95),
	DEFINE_IWAD("The Ultimate Doom v1.9 (Xbox Doom 3 bundle)", 0x0c8758f1, 0x02ccafe2, 0x6a3040be, 0xe8ba5021),
	DEFINE_IWAD("Hexen: Deathkings of the Dark Citadel v1.0", 0x1077432e, 0x2690d390, 0xc256ac90, 0x8b5f4efa),
	DEFINE_IWAD("Doom Registered v1.8", 0x11e1cd21, 0x6801ea26, 0x57723abc, 0x86ecb01f),
	DEFINE_IWAD("HacX v1.0", 0x1511a703, 0x2ebc834a, 0x3884cf39, 0x0d7f186e),
	DEFINE_IWAD("Doom Shareware v1.25", 0x17aebd6b, 0x5f2ed8ce, 0x07aa526a, 0x32af8d99),
	DEFINE_IWAD("Action Doom 2: Urban Brawl v1.0", 0x1914b280, 0xb0a4b517, 0x214523bc, 0x2270e758),
	DEFINE_IWAD("Doom Registered v1.9", 0x1cd63c5d, 0xdff1bf8c, 0xe844237f, 0x580e9cf3),
	DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (id Anthology)", 0x1d39e405, 0xbf6ee3df, 0x69a8d264, 0x6c8d5c49),
	DEFINE_IWAD("Heretic Registered v1.2", 0x1e4cb4ef, 0x075ad344, 0xdd639716, 0x37307e04),
	DEFINE_IWAD("Chex Quest v1.0", 0x25485721, 0x882b050a, 0xfa96a56e, 0x5758dd52),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9", 0x25e1459c, 0xa71d3215, 0x25f84628, 0xf45ca8cd),
	DEFINE_IWAD("Strife: Veteran Edition v1.1", 0x2c0a712d, 0x3e39b010, 0x519c879f, 0x734d79ae),
	DEFINE_IWAD("Strife Registered v1.2+", 0x2fed2031, 0xa5b03892, 0x106e0f11, 0x7f17901f),
	DEFINE_IWAD("Doom Shareware v1.2", 0x30aa5beb, 0x9e5ebfbb, 0xe1e17655, 0x61c08f38),
	DEFINE_IWAD("Doom II: Hell on Earth v1.666", 0x30e3c2d0, 0x350b67bf, 0xbf472719, 0x70b74b2f),
	DEFINE_IWAD("Heretic Registered v1.0", 0x3117e399, 0xcdb4298e, 0xaa394162, 0x5f4b2923),
	DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9 (id Anthology)", 0x3493be7e, 0x1e2588bc, 0x9c8b31ea, 0xb2587a04),
	DEFINE_IWAD("Doom II: Hell on Earth v1.8 (French release)", 0x3cb02349, 0xb3df649c, 0x86290907, 0xeed64e7b),
	DEFINE_IWAD("The Ultimate Doom v1.9 (Doom PDA)", 0x3e410ecd, 0x27f61437, 0xd53fa5c2, 0x79536e88),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (XBLA Doom II)", 0x43c2df32, 0xdc6c740c, 0xb11f34dc, 0x5ab693fa),
	DEFINE_IWAD("Doom Registered v1.6b", 0x464e3723, 0xa7e7f970, 0x39ac9fd0, 0x57096adb),
	DEFINE_IWAD("Strife: Veteran Edition v1.2+", 0x47958a4f, 0xea8a5411, 0x6e4b51fc, 0x155799c0),
	DEFINE_IWAD("Harmony v1.1", 0x48ebb49b, 0x52f6a302, 0x0d174dbc, 0xc1b9aeaf),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (PSN Classic Complete)", 0x4c3db5f2, 0x3b145fcc, 0xd24c9b84, 0xaba3b7dd),
	DEFINE_IWAD("Final Doom: TNT Evilution v1.9", 0x4e158d99, 0x53c79ccf, 0x97bd0663, 0x244cc6b6),
	DEFINE_IWAD("Doom Shareware v1.1", 0x52cbc888, 0x2f445573, 0xce421fa5, 0x453513c1),
	DEFINE_IWAD("Doom Registered v1.666", 0x54978d12, 0xde87f162, 0xb9bcc011, 0x676cb3c0),
	DEFINE_IWAD("Chex Quest 3 v1.0", 0x59c98599, 0x5db55cd2, 0x623c1893, 0x550d82b3),
	DEFINE_IWAD("Doom Shareware v1.8", 0x5f4eb849, 0xb1af1288, 0x7dec04a2, 0xa12e5e62),
	DEFINE_IWAD("HacX v1.2", 0x65ed74d5, 0x22bdf664, 0x9c2831b1, 0x3b9e02b4),
	DEFINE_IWAD("Heretic Registered v1.3", 0x66d686b1, 0xed6d35ff, 0x103f15db, 0xd30e0341),
	DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (DOOMPatcher)", 0x677605e1, 0xa7ee75dc, 0x27937303, 0x6cdb6ebb),
	DEFINE_IWAD("The Ultimate Doom v1.9 (XBLA Doom)", 0x72286ddc, 0x680d47b9, 0x138053dd, 0x944b2a3d),
	DEFINE_IWAD("Doom Alpha v0.2", 0x74090111, 0x9ba2953e, 0x3c7f3764, 0xeca6e128),
	DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9", 0x75c8cf89, 0x566741fa, 0x9d224476, 0x04053bd7),
	DEFINE_IWAD("Doom Shareware v1.6b", 0x762fd6d4, 0xb960d4b7, 0x59730f01, 0x387a50a1),
	DEFINE_IWAD("Hexen: Deathkings of the Dark Citadel v1.1", 0x78d5898e, 0x99e220e4, 0xde64edaa, 0x0e479593),
	DEFINE_IWAD("The Ultimate Doom v1.9 (Xbox 360 BFG Edition)", 0x7912931e, 0x44c7d56e, 0x021084a2, 0x56659800),
	DEFINE_IWAD("Doom Registered v1.2", 0x792fd1fe, 0xa023d612, 0x10857089, 0xa7c1e351),
	DEFINE_IWAD("Master Levels for Doom II (PSN Classic Complete)", 0x84cb8640, 0xf599c4a1, 0x7c8eb526, 0xf90d2b7a),
	DEFINE_IWAD("Hexen Shareware v1.0", 0x876a5a44, 0xc7b68f04, 0xb3bb9bc7, 0xa5bd69d6),
	DEFINE_IWAD("Strife Registered v1.1", 0x8f2d3a6a, 0x289f5d2f, 0x2f9c1eec, 0x02b47299),
	DEFINE_IWAD("Doom Shareware v0.99/v1.0", 0x90facab2, 0x1eede798, 0x1be10790, 0xe3f82da2),
	DEFINE_IWAD("Hexen Shareware Beta", 0x9178a32a, 0x496ff5be, 0xfebfe6c4, 0x7dac106c),
	DEFINE_IWAD("Hexen Shareware v1.0 (Mac)", 0x925f9f50, 0x00e17dc8, 0x4b0a6a3b, 0xed3a6f31),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Tapwave Zodiac)", 0x9640fc4b, 0x2c8447bb, 0xd28f2080, 0x725d5c51),
	DEFINE_IWAD("Doom Registered v1.1", 0x981b03e6, 0xd1dc0333, 0x01aa3095, 0xacc437ce),
	DEFINE_IWAD("Doom Alpha v0.5", 0x9c877480, 0xb8ef33b7, 0x074f1f0c, 0x07ed6487),
	DEFINE_IWAD("Doom Shareware v1.4b", 0xa21ae40c, 0x388cb6f2, 0xc3cc1b95, 0x589ee693),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Xbox Doom 3 bundle)", 0xa793ebcd, 0xd790afad, 0x4a1f39cc, 0x39a893bd),
	DEFINE_IWAD("Hexen Registered v1.1", 0xabb033ca, 0xf81e26f1, 0x2a2103e1, 0xfa25453f),
	DEFINE_IWAD("Heretic Shareware v1.2", 0xae779722, 0x390ec32f, 0xa37b0d36, 0x1f7d82f8),
	DEFINE_IWAD("Hexen Registered v1.0", 0xb2543a03, 0x52136526, 0x1d0a0f74, 0xd5dd90f0),
	DEFINE_IWAD("Hexen Registered v1.1 (Mac)", 0xb68140a7, 0x96f6fd7f, 0x3a5d3226, 0xa32b93be),
	DEFINE_IWAD("Doom Alpha v0.4", 0xb6afa12a, 0x8b22e272, 0x6a8ff5bd, 0x249223de),
	DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9 (PSN Classic Complete)", 0xb77ca6a8, 0x09c4fae0, 0x86162dad, 0x8e7a1335),
	DEFINE_IWAD("HacX v1.1", 0xb7fd2f43, 0xf3382cf0, 0x12dc6b09, 0x7a3cb182),
	DEFINE_IWAD("Strife Shareware v1.1", 0xbb545b9c, 0x4eca0ff9, 0x2c14d466, 0xb3294023),
	DEFINE_IWAD("Chex Quest 3 v1.4", 0xbce163d0, 0x6521f9d1, 0x5f968678, 0x6e64df13),
	DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (PSN Classic Complete)", 0xbe626c12, 0xb7c9d94b, 0x1dfb9c32, 0x7566b4ff),
	DEFINE_IWAD("Action Doom 2: Urban Brawl v1.1", 0xc106a4e0, 0xa96f2999, 0x54b073d5, 0xf97240be),
	DEFINE_IWAD("Doom II: Hell on Earth v1.8", 0xc236745b, 0xb01d89bb, 0xb866c8fe, 0xd81b6f8c),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (BFG Edition)", 0xc3bea405, 0x70c23e51, 0x1a7ed3eb, 0xcd9865f7),
	DEFINE_IWAD("Doom Shareware v1.666", 0xc428ea39, 0x4dc52835, 0xf2580d5b, 0xfd50d76f),
	DEFINE_IWAD("The Ultimate Doom v1.9", 0xc4fe9fd9, 0x20207691, 0xa9f49366, 0x8e0a2083),
	DEFINE_IWAD("Hexen Registered Beta", 0xc88a2bb3, 0xd783e2ad, 0x7b599a8e, 0x301e099e),
	DEFINE_IWAD("Doom Shareware v1.1 (cancelled release)", 0xcea4989d, 0xf52b65f4, 0xd481b706, 0x234a3dca),
	DEFINE_IWAD("Doom II: Hell on Earth v1.7a", 0xd7a07e5d, 0x3f462507, 0x4312bc29, 0x9d7ed33f),
	DEFINE_IWAD("Doom II: Hell on Earth v1.666 (German release)", 0xd9153ced, 0x9fd5b898, 0xb36cc584, 0x4e35b520),
	DEFINE_IWAD("The Ultimate Doom v1.9 (Doom PDA)", 0xdae77aff, 0x77a0491e, 0x3b7254c9, 0xc8401aa8),
	DEFINE_IWAD("Doom Alpha v0.3", 0xdae9b1ee, 0xa1a8e090, 0xfdfa5707, 0x187f4a43),
	DEFINE_IWAD("Strife Shareware v1.0", 0xde2c8dca, 0xd7cca206, 0x292294bd, 0xab524292),
	DEFINE_IWAD("Doom Shareware v1.5b", 0xe280233d, 0x533dcc28, 0xc1acd6cc, 0xdc7742d4),
	DEFINE_IWAD("The Ultimate Doom v1.9 (PSN Classic Complete)", 0xe4f120ea, 0xb6fb410a, 0x5b6e11c9, 0x47832357),
	DEFINE_IWAD("Doom II: Hell on Earth v1.7", 0xea74a47a, 0x791fdef2, 0xe9f2ea8b, 0x8a9da13b),
	DEFINE_IWAD("Doom Shareware v1.9", 0xf0cefca4, 0x9926d009, 0x03cf5755, 0x1d901abe),
	DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Xbox 360 BFG Edition)", 0xf617591a, 0x6c5d0703, 0x7eb716dc, 0x4863e26b),
	DEFINE_IWAD("The Ultimate Doom v1.9 (BFG Edition)", 0xfb35c4a5, 0xa9fd49ec, 0x29ab6e90, 0x0572c524),
	DEFINE_IWAD("Heretic Shareware Beta", 0xfc7eab65, 0x9f6ee522, 0xbb57acc1, 0xa946912f),
	DEFINE_IWAD("Chex Quest 2 v1.0", 0xfdc4ffa5, 0x7e1983e3, 0x0912c006, 0x284a3e01),
	DEFINE_IWAD("Harmony v1.0", 0xfe2cce67, 0x13ddcf6c, 0x6d6f0e81, 0x54b0cb38)
};

#define SORTED_IWAD_HASHES_SIZE	(sizeof(sorted_iwad_hashes)/sizeof(IwadHash))

bool IwadHashCompare(const IwadHash &left, const IwadHash &right)
{
	return memcmp(left.md5, right.md5, 16)<0;
}

struct FileDesc {
	const char* file;
	const char* name;
};

//This is sorted list of IWAD files
//It acts as fallback for the list of IWAD hashes
//Original unsorted list, that is easier to manage, is provided in comments at the bottom of this source file
const FileDesc sorted_iwad_files[]={
	{"action2.wad", "Action Doom 2: Urban Brawl"},
	{"bfgdoom.wad", "The Ultimate Doom (BFG Edition)"},
	{"bfgdoom2.wad", "Doom II: Hell on Earth (BFG Edition)"},
	{"blasphem.wad", "Blasphemer"},
	{"blasphemer.wad", "Blasphemer"},
	{"chex.wad", "Chex Quest"},
	{"chex3.wad", "Chex Quest 3"},
	{"delaweare.wad", "Delaweare"},
	{"doom_complete.pk3", "DOOM Complete (WADSmoosh)"},
	{"doom.wad", "Doom Registered"},
	{"doom1.wad", "Doom Shareware"},
	{"doom2.wad", "Doom II: Hell on Earth"},
	{"doom2bfg.wad", "Doom II: Hell on Earth (BFG Edition)"},
	{"doom2f.wad", "Doom II: Hell on Earth (French release)"},
	{"doombfg.wad", "The Ultimate Doom (BFG Edition)"},
	{"doomu.wad", "The Ultimate Doom"},
	{"freedm.wad", "FreeDM"},
	{"freedoom.wad", "Freedoom: Phase 2"},
	{"freedoom1.wad", "Freedoom: Phase 1"},
	{"freedoom2.wad", "Freedoom: Phase 2"},
	{"freedoomu.wad", "Freedoom: Phase 1"},
	{"hacx.wad", "HacX v2.0"},
	{"hacx2.wad", "HacX v2.0"},
	{"harm1.wad", "Harmony"},
	{"heretic.wad", "Heretic Registered"},
	{"heretic1.wad", "Heretic Shareware"},
	{"hereticsr.wad", "Heretic Registered"},
	{"hexdd.wad", "Hexen: Deathkings of the Dark Citadel"},
	{"hexdemo.wad", "Hexen Shareware"},
	{"hexen.wad", "Hexen Registered"},
	{"hexendemo.wad", "Hexen Shareware"},
	{"plutonia.wad", "Final Doom: The Plutonia Experiment"},
	{"rotwb.wad", "Rise Of The Wool Ball"},
	{"square.pk3", "The Adventures of Square"},
	{"square1.pk3", "The Adventures of Square"},
	{"strife.wad", "Strife Registered"},
	{"strife0.wad", "Strife Shareware"},
	{"strife1.wad", "Strife Registered"},
	{"sve.wad", "Strife: Veteran Edition"},
	{"tnt.wad", "Final Doom: TNT Evilution"},
	{"tntyk.wad", "Final Doom: TNT Evilution (DOOMPatcher)"},
	{"voices.wad", "Strife Voices WAD"}
};

#define SORTED_IWAD_FILES_SIZE	(sizeof(sorted_iwad_files)/sizeof(FileDesc))

//This is sorted list of source ports binaries
//It's not a list of ZDL supported source ports but a list of more or less popular source ports
//Original unsorted list, that is easier to manage, is provided in comments at the bottom of this source file
const FileDesc sorted_source_ports[]={
	{"boom", "Boom"},
	{"chocolate-doom", "Chocolate Doom"},
	{"chocolate-heretic", "Chocolate Heretic"},
	{"chocolate-hexen", "Chocolate Hexen"},
	{"chocolate-strife", "Chocolate Strife"},
	{"cndoom", "CnDoom (Doom)"},
	{"cnheretic", "CnDoom (Heretic)"},
	{"cnhexen", "CnDoom (Hexen)"},
	{"cnserver", "CnDoom (Server)"},
	{"cnstrife", "CnDoom (Strife)"},
	{"crispy-doom", "Crispy Doom"},
	{"crispy-server", "Crispy Doom (Server)"},
	{"doom", "Doom"},
	{"doom2", "Doom II: Hell on Earth"},
	{"doom3d", "Doom3D"},
	{"doom95", "Doom 95"},
	{"doomcl", "csDoom"},
	{"doomgl", "DoomGL"},
	{"doomlegacy", "Doom Legacy"},
	{"doomplus", "Doom Plus"},
	{"doomretro", "Doom Retro"},
	{"doomsday", "Doomsday Engine"},
	{"doomsday-server", "Doomsday Engine (Server)"},
	{"dosdoom", "DOSDoom"},
	{"edge", "EDGE"},
	{"eternity", "Eternity Engine"},
	{"glboom", "PrBoom (OpenGL)"},
	{"glboom-plus", "PrBoom+ (OpenGL)"},
	{"gldoom", "GLDoom"},
	{"gzdoom", "GZDoom"},
	{"heretic", "Heretic"},
	{"hexen", "Hexen"},
	{"hexendk", "Hexen: Deathkings of the Dark Citadel"},
	{"jdoom", "jDoom (Doom)"},
	{"jheretic", "jDoom (Heretic)"},
	{"jhexen", "jDoom (Hexen)"},
	{"linuxsdoom", "Linux Doom (SVGAlib)"},
	{"linuxxdoom", "Linux Doom (X)"},
	{"lxdoom", "LxDoom"},
	{"lxdoom-game-server", "LxDoom (Server)"},
	{"mbf", "Marine's Best Friend (MBF)"},
	{"mochadoom", "Mocha Doom"},
	{"mochadoom7", "Mocha Doom (Win 7)"},
	{"odamex", "Odamex"},
	{"odasrv", "Odamex (Server)"},
	{"prboom", "PrBoom"},
	{"prboom_server", "PrBoom (Server)"},
	{"prboom-plus", "PrBoom+"},
	{"prboom-plus_server", "PrBoom+ (Server)"},
	{"qzdoom", "QZDoom"},
	{"remood", "ReMooD"},
	{"risen3d", "Risen3D"},
	{"skulltag", "Skulltag"},
	{"smmu", "Smack My Marine Up (SMMU)"},
	{"strawberry-doom", "Strawberry Doom"},
	{"strawberry-server", "Strawberry Doom (Server)"},
	{"strife", "Strife (Shareware)"},
	{"strife1", "Strife"},
	{"tasdoom", "TASDoom"},
	{"vavoom", "Vavoom"},
	{"vavoom-dedicated", "Vavoom (Server)"},
	{"wdmp", "WDMP"},
	{"wdmp32s", "WDMP (Win32s)"},
	{"windoom", "WinDoom"},
	{"zandronum", "Zandronum"},
	{"zdaemon", "ZDaemon"},
	{"zdaemongl", "ZDaemonGL"},
	{"zdoom", "ZDoom"},
	{"zdoomgl", "ZDoomGL"},
	{"zserv32", "ZDaemon (Server)"}
};

#define SORTED_SOURCE_PORTS_SIZE	(sizeof(sorted_source_ports)/sizeof(FileDesc))

bool FileDescCompare(const FileDesc &left, const FileDesc &right)
{
	return strcmp(left.file, right.file)<0;
}

ZDLFileInfo::ZDLFileInfo():
	QFileInfo()
{}

ZDLFileInfo::ZDLFileInfo(const QString &file):
	QFileInfo(file)
{}

QString ZDLFileInfo::GetFileDescription()
{
	return baseName();
}

ZDLIwadInfo::ZDLIwadInfo():
	ZDLFileInfo()
{}

ZDLIwadInfo::ZDLIwadInfo(const QString &file):
	ZDLFileInfo(file)
{}

QString ZDLIwadInfo::GetFileDescription()
{
	QString iwad_name;

	QFile iwad_file(filePath());
	if (iwad_file.open(QFile::ReadOnly)) {
		QCryptographicHash file_hash(QCryptographicHash::Md5);
		char buffer[1024];
		int length;

		while ((length=iwad_file.read(buffer, sizeof(buffer)))>0)
			file_hash.addData(buffer, length);

		if (iwad_file.atEnd()) {
			IwadHash target_hash;
			memcpy(target_hash.md5, file_hash.result().data(), 16);

			const IwadHash *null_iwad=sorted_iwad_hashes+SORTED_IWAD_HASHES_SIZE;
			const IwadHash *found_iwad=std::lower_bound(sorted_iwad_hashes, null_iwad, target_hash, IwadHashCompare);

			if (found_iwad!=null_iwad&&!IwadHashCompare(target_hash, *found_iwad))
				iwad_name=found_iwad->name;
		}

		iwad_file.close();
	}

	ZDLMapFile *mapfile;
	if (iwad_name.isEmpty()&&(mapfile=ZDLMapFile::getMapFile(filePath()))) {
		iwad_name=mapfile->getIwadinfoName();
		delete mapfile;
	}

	if (iwad_name.isEmpty()) {
		std::string file_buf=fileName().toLower().toStdString();
		FileDesc target_file={file_buf.c_str()};

		const FileDesc *null_iwad=sorted_iwad_files+SORTED_IWAD_FILES_SIZE;
		const FileDesc *found_iwad=std::lower_bound(sorted_iwad_files, null_iwad, target_file, FileDescCompare);

		if (found_iwad!=null_iwad&&!FileDescCompare(target_file, *found_iwad))
			iwad_name=found_iwad->name;
	}

	return iwad_name.isEmpty()?fileName().toUpper():iwad_name;
}

ZDLAppInfo::ZDLAppInfo():
	ZDLFileInfo()
{}

ZDLAppInfo::ZDLAppInfo(const QString &file):
	ZDLFileInfo(file)
{}

QString ZDLAppInfo::GetFileDescription()
{
	std::string file_buf=baseName().toLower().toStdString();
	FileDesc target_file={file_buf.c_str()};

	const FileDesc *null_port=sorted_source_ports+SORTED_SOURCE_PORTS_SIZE;
	const FileDesc *found_port=std::lower_bound(sorted_source_ports, null_port, target_file, FileDescCompare);

	if (found_port!=null_port&&!FileDescCompare(target_file, *found_port))
		return found_port->name;
	else
		return baseName();
}

/**********************************************************
//This is unsorted IWAD hash list
//Simply copy this to Excel or Google Tables, edit any hashes, add new IWADs and sort it using first column
//Then copy second and third columns to sorted_iwad_hashes
//Don't forget to remove comma at the end of the last element
//N.B.: List actually contains some PWADs for the simplicity of comparing it to other "commercial WAD lists" that often include both IWADs and PWADs

740901119ba2953e3c7f3764eca6e128		DEFINE_IWAD("Doom Alpha v0.2", 0x74090111, 0x9ba2953e, 0x3c7f3764, 0xeca6e128),
dae9b1eea1a8e090fdfa5707187f4a43		DEFINE_IWAD("Doom Alpha v0.3", 0xdae9b1ee, 0xa1a8e090, 0xfdfa5707, 0x187f4a43),
b6afa12a8b22e2726a8ff5bd249223de		DEFINE_IWAD("Doom Alpha v0.4", 0xb6afa12a, 0x8b22e272, 0x6a8ff5bd, 0x249223de),
9c877480b8ef33b7074f1f0c07ed6487		DEFINE_IWAD("Doom Alpha v0.5", 0x9c877480, 0xb8ef33b7, 0x074f1f0c, 0x07ed6487),
049e32f18d9c9529630366cfc72726ea		DEFINE_IWAD("Doom Press Release Beta", 0x049e32f1, 0x8d9c9529, 0x630366cf, 0xc72726ea),
90facab21eede7981be10790e3f82da2		DEFINE_IWAD("Doom Shareware v0.99/v1.0", 0x90facab2, 0x1eede798, 0x1be10790, 0xe3f82da2),
cea4989df52b65f4d481b706234a3dca		DEFINE_IWAD("Doom Shareware v1.1 (cancelled release)", 0xcea4989d, 0xf52b65f4, 0xd481b706, 0x234a3dca),
52cbc8882f445573ce421fa5453513c1		DEFINE_IWAD("Doom Shareware v1.1", 0x52cbc888, 0x2f445573, 0xce421fa5, 0x453513c1),
30aa5beb9e5ebfbbe1e1765561c08f38		DEFINE_IWAD("Doom Shareware v1.2", 0x30aa5beb, 0x9e5ebfbb, 0xe1e17655, 0x61c08f38),
17aebd6b5f2ed8ce07aa526a32af8d99		DEFINE_IWAD("Doom Shareware v1.25", 0x17aebd6b, 0x5f2ed8ce, 0x07aa526a, 0x32af8d99),
a21ae40c388cb6f2c3cc1b95589ee693		DEFINE_IWAD("Doom Shareware v1.4b", 0xa21ae40c, 0x388cb6f2, 0xc3cc1b95, 0x589ee693),
e280233d533dcc28c1acd6ccdc7742d4		DEFINE_IWAD("Doom Shareware v1.5b", 0xe280233d, 0x533dcc28, 0xc1acd6cc, 0xdc7742d4),
762fd6d4b960d4b759730f01387a50a1		DEFINE_IWAD("Doom Shareware v1.6b", 0x762fd6d4, 0xb960d4b7, 0x59730f01, 0x387a50a1),
c428ea394dc52835f2580d5bfd50d76f		DEFINE_IWAD("Doom Shareware v1.666", 0xc428ea39, 0x4dc52835, 0xf2580d5b, 0xfd50d76f),
5f4eb849b1af12887dec04a2a12e5e62		DEFINE_IWAD("Doom Shareware v1.8", 0x5f4eb849, 0xb1af1288, 0x7dec04a2, 0xa12e5e62),
f0cefca49926d00903cf57551d901abe		DEFINE_IWAD("Doom Shareware v1.9", 0xf0cefca4, 0x9926d009, 0x03cf5755, 0x1d901abe),
981b03e6d1dc033301aa3095acc437ce		DEFINE_IWAD("Doom Registered v1.1", 0x981b03e6, 0xd1dc0333, 0x01aa3095, 0xacc437ce),
792fd1fea023d61210857089a7c1e351		DEFINE_IWAD("Doom Registered v1.2", 0x792fd1fe, 0xa023d612, 0x10857089, 0xa7c1e351),
464e3723a7e7f97039ac9fd057096adb		DEFINE_IWAD("Doom Registered v1.6b", 0x464e3723, 0xa7e7f970, 0x39ac9fd0, 0x57096adb),
54978d12de87f162b9bcc011676cb3c0		DEFINE_IWAD("Doom Registered v1.666", 0x54978d12, 0xde87f162, 0xb9bcc011, 0x676cb3c0),
11e1cd216801ea2657723abc86ecb01f		DEFINE_IWAD("Doom Registered v1.8", 0x11e1cd21, 0x6801ea26, 0x57723abc, 0x86ecb01f),
1cd63c5ddff1bf8ce844237f580e9cf3		DEFINE_IWAD("Doom Registered v1.9", 0x1cd63c5d, 0xdff1bf8c, 0xe844237f, 0x580e9cf3),
c4fe9fd920207691a9f493668e0a2083		DEFINE_IWAD("The Ultimate Doom v1.9", 0xc4fe9fd9, 0x20207691, 0xa9f49366, 0x8e0a2083),
fb35c4a5a9fd49ec29ab6e900572c524		DEFINE_IWAD("The Ultimate Doom v1.9 (BFG Edition)", 0xfb35c4a5, 0xa9fd49ec, 0x29ab6e90, 0x0572c524),
7912931e44c7d56e021084a256659800		DEFINE_IWAD("The Ultimate Doom v1.9 (Xbox 360 BFG Edition)", 0x7912931e, 0x44c7d56e, 0x021084a2, 0x56659800),
0c8758f102ccafe26a3040bee8ba5021		DEFINE_IWAD("The Ultimate Doom v1.9 (Xbox Doom 3 bundle)", 0x0c8758f1, 0x02ccafe2, 0x6a3040be, 0xe8ba5021),
72286ddc680d47b9138053dd944b2a3d		DEFINE_IWAD("The Ultimate Doom v1.9 (XBLA Doom)", 0x72286ddc, 0x680d47b9, 0x138053dd, 0x944b2a3d),
e4f120eab6fb410a5b6e11c947832357		DEFINE_IWAD("The Ultimate Doom v1.9 (PSN Classic Complete)", 0xe4f120ea, 0xb6fb410a, 0x5b6e11c9, 0x47832357),
3e410ecd27f61437d53fa5c279536e88		DEFINE_IWAD("The Ultimate Doom v1.9 (Doom PDA)", 0x3e410ecd, 0x27f61437, 0xd53fa5c2, 0x79536e88),
dae77aff77a0491e3b7254c9c8401aa8		DEFINE_IWAD("The Ultimate Doom v1.9 (Doom PDA)", 0xdae77aff, 0x77a0491e, 0x3b7254c9, 0xc8401aa8),
d9153ced9fd5b898b36cc5844e35b520		DEFINE_IWAD("Doom II: Hell on Earth v1.666 (German release)", 0xd9153ced, 0x9fd5b898, 0xb36cc584, 0x4e35b520),
30e3c2d0350b67bfbf47271970b74b2f		DEFINE_IWAD("Doom II: Hell on Earth v1.666", 0x30e3c2d0, 0x350b67bf, 0xbf472719, 0x70b74b2f),
ea74a47a791fdef2e9f2ea8b8a9da13b		DEFINE_IWAD("Doom II: Hell on Earth v1.7", 0xea74a47a, 0x791fdef2, 0xe9f2ea8b, 0x8a9da13b),
d7a07e5d3f4625074312bc299d7ed33f		DEFINE_IWAD("Doom II: Hell on Earth v1.7a", 0xd7a07e5d, 0x3f462507, 0x4312bc29, 0x9d7ed33f),
3cb02349b3df649c86290907eed64e7b		DEFINE_IWAD("Doom II: Hell on Earth v1.8 (French release)", 0x3cb02349, 0xb3df649c, 0x86290907, 0xeed64e7b),
c236745bb01d89bbb866c8fed81b6f8c		DEFINE_IWAD("Doom II: Hell on Earth v1.8", 0xc236745b, 0xb01d89bb, 0xb866c8fe, 0xd81b6f8c),
25e1459ca71d321525f84628f45ca8cd		DEFINE_IWAD("Doom II: Hell on Earth v1.9", 0x25e1459c, 0xa71d3215, 0x25f84628, 0xf45ca8cd),
c3bea40570c23e511a7ed3ebcd9865f7		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (BFG Edition)", 0xc3bea405, 0x70c23e51, 0x1a7ed3eb, 0xcd9865f7),
f617591a6c5d07037eb716dc4863e26b		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Xbox 360 BFG Edition)", 0xf617591a, 0x6c5d0703, 0x7eb716dc, 0x4863e26b),
a793ebcdd790afad4a1f39cc39a893bd		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Xbox Doom 3 bundle)", 0xa793ebcd, 0xd790afad, 0x4a1f39cc, 0x39a893bd),
43c2df32dc6c740cb11f34dc5ab693fa		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (XBLA Doom II)", 0x43c2df32, 0xdc6c740c, 0xb11f34dc, 0x5ab693fa),
4c3db5f23b145fccd24c9b84aba3b7dd		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (PSN Classic Complete)", 0x4c3db5f2, 0x3b145fcc, 0xd24c9b84, 0xaba3b7dd),
9640fc4b2c8447bbd28f2080725d5c51		DEFINE_IWAD("Doom II: Hell on Earth v1.9 (Tapwave Zodiac)", 0x9640fc4b, 0x2c8447bb, 0xd28f2080, 0x725d5c51),
75c8cf89566741fa9d22447604053bd7		DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9", 0x75c8cf89, 0x566741fa, 0x9d224476, 0x04053bd7),
3493be7e1e2588bc9c8b31eab2587a04		DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9 (id Anthology)", 0x3493be7e, 0x1e2588bc, 0x9c8b31ea, 0xb2587a04),
b77ca6a809c4fae086162dad8e7a1335		DEFINE_IWAD("Final Doom: The Plutonia Experiment v1.9 (PSN Classic Complete)", 0xb77ca6a8, 0x09c4fae0, 0x86162dad, 0x8e7a1335),
4e158d9953c79ccf97bd0663244cc6b6		DEFINE_IWAD("Final Doom: TNT Evilution v1.9", 0x4e158d99, 0x53c79ccf, 0x97bd0663, 0x244cc6b6),
677605e1a7ee75dc279373036cdb6ebb		DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (DOOMPatcher)", 0x677605e1, 0xa7ee75dc, 0x27937303, 0x6cdb6ebb),
1d39e405bf6ee3df69a8d2646c8d5c49		DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (id Anthology)", 0x1d39e405, 0xbf6ee3df, 0x69a8d264, 0x6c8d5c49),
be626c12b7c9d94b1dfb9c327566b4ff		DEFINE_IWAD("Final Doom: TNT Evilution v1.9 (PSN Classic Complete)", 0xbe626c12, 0xb7c9d94b, 0x1dfb9c32, 0x7566b4ff),
84cb8640f599c4a17c8eb526f90d2b7a		DEFINE_IWAD("Master Levels for Doom II (PSN Classic Complete)", 0x84cb8640, 0xf599c4a1, 0x7c8eb526, 0xf90d2b7a),
fc7eab659f6ee522bb57acc1a946912f		DEFINE_IWAD("Heretic Shareware Beta", 0xfc7eab65, 0x9f6ee522, 0xbb57acc1, 0xa946912f),
023b52175d2f260c3bdc5528df5d0a8c		DEFINE_IWAD("Heretic Shareware v1.0", 0x023b5217, 0x5d2f260c, 0x3bdc5528, 0xdf5d0a8c),
ae779722390ec32fa37b0d361f7d82f8		DEFINE_IWAD("Heretic Shareware v1.2", 0xae779722, 0x390ec32f, 0xa37b0d36, 0x1f7d82f8),
3117e399cdb4298eaa3941625f4b2923		DEFINE_IWAD("Heretic Registered v1.0", 0x3117e399, 0xcdb4298e, 0xaa394162, 0x5f4b2923),
1e4cb4ef075ad344dd63971637307e04		DEFINE_IWAD("Heretic Registered v1.2", 0x1e4cb4ef, 0x075ad344, 0xdd639716, 0x37307e04),
66d686b1ed6d35ff103f15dbd30e0341		DEFINE_IWAD("Heretic Registered v1.3", 0x66d686b1, 0xed6d35ff, 0x103f15db, 0xd30e0341),
9178a32a496ff5befebfe6c47dac106c		DEFINE_IWAD("Hexen Shareware Beta", 0x9178a32a, 0x496ff5be, 0xfebfe6c4, 0x7dac106c),
876a5a44c7b68f04b3bb9bc7a5bd69d6		DEFINE_IWAD("Hexen Shareware v1.0", 0x876a5a44, 0xc7b68f04, 0xb3bb9bc7, 0xa5bd69d6),
925f9f5000e17dc84b0a6a3bed3a6f31		DEFINE_IWAD("Hexen Shareware v1.0 (Mac)", 0x925f9f50, 0x00e17dc8, 0x4b0a6a3b, 0xed3a6f31),
c88a2bb3d783e2ad7b599a8e301e099e		DEFINE_IWAD("Hexen Registered Beta", 0xc88a2bb3, 0xd783e2ad, 0x7b599a8e, 0x301e099e),
b2543a03521365261d0a0f74d5dd90f0		DEFINE_IWAD("Hexen Registered v1.0", 0xb2543a03, 0x52136526, 0x1d0a0f74, 0xd5dd90f0),
abb033caf81e26f12a2103e1fa25453f		DEFINE_IWAD("Hexen Registered v1.1", 0xabb033ca, 0xf81e26f1, 0x2a2103e1, 0xfa25453f),
b68140a796f6fd7f3a5d3226a32b93be		DEFINE_IWAD("Hexen Registered v1.1 (Mac)", 0xb68140a7, 0x96f6fd7f, 0x3a5d3226, 0xa32b93be),
1077432e2690d390c256ac908b5f4efa		DEFINE_IWAD("Hexen: Deathkings of the Dark Citadel v1.0", 0x1077432e, 0x2690d390, 0xc256ac90, 0x8b5f4efa),
78d5898e99e220e4de64edaa0e479593		DEFINE_IWAD("Hexen: Deathkings of the Dark Citadel v1.1", 0x78d5898e, 0x99e220e4, 0xde64edaa, 0x0e479593),
de2c8dcad7cca206292294bdab524292		DEFINE_IWAD("Strife Shareware v1.0", 0xde2c8dca, 0xd7cca206, 0x292294bd, 0xab524292),
bb545b9c4eca0ff92c14d466b3294023		DEFINE_IWAD("Strife Shareware v1.1", 0xbb545b9c, 0x4eca0ff9, 0x2c14d466, 0xb3294023),
8f2d3a6a289f5d2f2f9c1eec02b47299		DEFINE_IWAD("Strife Registered v1.1", 0x8f2d3a6a, 0x289f5d2f, 0x2f9c1eec, 0x02b47299),
2fed2031a5b03892106e0f117f17901f		DEFINE_IWAD("Strife Registered v1.2+", 0x2fed2031, 0xa5b03892, 0x106e0f11, 0x7f17901f),
06a8f99b9b756ac908917c3868b8e3bc		DEFINE_IWAD("Strife: Veteran Edition v1.0", 0x06a8f99b, 0x9b756ac9, 0x08917c38, 0x68b8e3bc),
2c0a712d3e39b010519c879f734d79ae		DEFINE_IWAD("Strife: Veteran Edition v1.1", 0x2c0a712d, 0x3e39b010, 0x519c879f, 0x734d79ae),
47958a4fea8a54116e4b51fc155799c0		DEFINE_IWAD("Strife: Veteran Edition v1.2+", 0x47958a4f, 0xea8a5411, 0x6e4b51fc, 0x155799c0),
082234d6a3f7086424856478b5aa9e95		DEFINE_IWAD("Strife Voices WAD", 0x082234d6, 0xa3f70864, 0x24856478, 0xb5aa9e95),
25485721882b050afa96a56e5758dd52		DEFINE_IWAD("Chex Quest v1.0", 0x25485721, 0x882b050a, 0xfa96a56e, 0x5758dd52),
fdc4ffa57e1983e30912c006284a3e01		DEFINE_IWAD("Chex Quest 2 v1.0", 0xfdc4ffa5, 0x7e1983e3, 0x0912c006, 0x284a3e01),
59c985995db55cd2623c1893550d82b3		DEFINE_IWAD("Chex Quest 3 v1.0", 0x59c98599, 0x5db55cd2, 0x623c1893, 0x550d82b3),
bce163d06521f9d15f9686786e64df13		DEFINE_IWAD("Chex Quest 3 v1.4", 0xbce163d0, 0x6521f9d1, 0x5f968678, 0x6e64df13),
1511a7032ebc834a3884cf390d7f186e		DEFINE_IWAD("HacX v1.0", 0x1511a703, 0x2ebc834a, 0x3884cf39, 0x0d7f186e),
b7fd2f43f3382cf012dc6b097a3cb182		DEFINE_IWAD("HacX v1.1", 0xb7fd2f43, 0xf3382cf0, 0x12dc6b09, 0x7a3cb182),
65ed74d522bdf6649c2831b13b9e02b4		DEFINE_IWAD("HacX v1.2", 0x65ed74d5, 0x22bdf664, 0x9c2831b1, 0x3b9e02b4),
1914b280b0a4b517214523bc2270e758		DEFINE_IWAD("Action Doom 2: Urban Brawl v1.0", 0x1914b280, 0xb0a4b517, 0x214523bc, 0x2270e758),
c106a4e0a96f299954b073d5f97240be		DEFINE_IWAD("Action Doom 2: Urban Brawl v1.1", 0xc106a4e0, 0xa96f2999, 0x54b073d5, 0xf97240be),
fe2cce6713ddcf6c6d6f0e8154b0cb38		DEFINE_IWAD("Harmony v1.0", 0xfe2cce67, 0x13ddcf6c, 0x6d6f0e81, 0x54b0cb38),
48ebb49b52f6a3020d174dbcc1b9aeaf		DEFINE_IWAD("Harmony v1.1", 0x48ebb49b, 0x52f6a302, 0x0d174dbc, 0xc1b9aeaf),

//This is unsorted IWAD file list
//Simply copy this to Excel or Google Tables, edit any filenames (should be in lowercase), add new IWADs and sort it using first column
//Then copy second and third columns to sorted_iwad_files
//Don't forget to remove comma at the end of the last element

doom1.wad		{"doom1.wad", "Doom Shareware"},
doom.wad		{"doom.wad", "Doom Registered"},
doomu.wad		{"doomu.wad", "The Ultimate Doom"},
bfgdoom.wad		{"bfgdoom.wad", "The Ultimate Doom (BFG Edition)"},
doombfg.wad		{"doombfg.wad", "The Ultimate Doom (BFG Edition)"},
doom2.wad		{"doom2.wad", "Doom II: Hell on Earth"},
doom2f.wad		{"doom2f.wad", "Doom II: Hell on Earth (French release)"},
bfgdoom2.wad		{"bfgdoom2.wad", "Doom II: Hell on Earth (BFG Edition)"},
doom2bfg.wad		{"doom2bfg.wad", "Doom II: Hell on Earth (BFG Edition)"},
plutonia.wad		{"plutonia.wad", "Final Doom: The Plutonia Experiment"},
tnt.wad		{"tnt.wad", "Final Doom: TNT Evilution"},
tntyk.wad		{"tntyk.wad", "Final Doom: TNT Evilution (DOOMPatcher)"},
heretic1.wad		{"heretic1.wad", "Heretic Shareware"},
heretic.wad		{"heretic.wad", "Heretic Registered"},
hereticsr.wad		{"hereticsr.wad", "Heretic Registered"},
hexdemo.wad		{"hexdemo.wad", "Hexen Shareware"},
hexendemo.wad		{"hexendemo.wad", "Hexen Shareware"},
hexen.wad		{"hexen.wad", "Hexen Registered"},
hexdd.wad		{"hexdd.wad", "Hexen: Deathkings of the Dark Citadel"},
strife0.wad		{"strife0.wad", "Strife Shareware"},
strife1.wad		{"strife1.wad", "Strife Registered"},
strife.wad		{"strife.wad", "Strife Registered"},
sve.wad		{"sve.wad", "Strife: Veteran Edition"},
voices.wad		{"voices.wad", "Strife Voices WAD"},
chex.wad		{"chex.wad", "Chex Quest"},
chex3.wad		{"chex3.wad", "Chex Quest 3"},
action2.wad		{"action2.wad", "Action Doom 2: Urban Brawl"},
harm1.wad		{"harm1.wad", "Harmony"},
hacx.wad		{"hacx.wad", "HacX v2.0"},
hacx2.wad		{"hacx2.wad", "HacX v2.0"},
freedoomu.wad		{"freedoomu.wad", "Freedoom: Phase 1"},
freedoom1.wad		{"freedoom1.wad", "Freedoom: Phase 1"},
freedoom.wad		{"freedoom.wad", "Freedoom: Phase 2"},
freedoom2.wad		{"freedoom2.wad", "Freedoom: Phase 2"},
freedm.wad		{"freedm.wad", "FreeDM"},
square.pk3		{"square.pk3", "The Adventures of Square"},
square1.pk3		{"square1.pk3", "The Adventures of Square"},
delaweare.wad		{"delaweare.wad", "Delaweare"},
rotwb.wad		{"rotwb.wad", "Rise Of The Wool Ball"},
doom_complete.pk3		{"doom_complete.pk3", "DOOM Complete (WADSmoosh)"},
blasphem.wad		{"blasphem.wad", "Blasphemer"},
blasphemer.wad		{"blasphemer.wad", "Blasphemer"},

//This is unsorted list of source ports binaries
//Simply copy this to Excel or Google Tables, edit any filenames (should be in lowercase w/o extension), add new source ports and sort it using first column
//Then copy second and third columns to sorted_source_ports
//Don't forget to remove comma at the end of the last element

doom		{"doom", "Doom"},
doom2		{"doom2", "Doom II: Hell on Earth"},
heretic		{"heretic", "Heretic"},
hexen		{"hexen", "Hexen"},
hexendk		{"hexendk", "Hexen: Deathkings of the Dark Citadel"},
strife		{"strife", "Strife (Shareware)"},
strife1		{"strife1", "Strife"},
windoom		{"windoom", "WinDoom"},
doom95		{"doom95", "Doom 95"},
zdoom		{"zdoom", "ZDoom"},
gzdoom		{"gzdoom", "GZDoom"},
qzdoom		{"qzdoom", "QZDoom"},
skulltag		{"skulltag", "Skulltag"},
doomsday		{"doomsday", "Doomsday Engine"},
doomsday-server		{"doomsday-server", "Doomsday Engine (Server)"},
chocolate-doom		{"chocolate-doom", "Chocolate Doom"},
chocolate-heretic		{"chocolate-heretic", "Chocolate Heretic"},
chocolate-hexen		{"chocolate-hexen", "Chocolate Hexen"},
chocolate-strife		{"chocolate-strife", "Chocolate Strife"},
cndoom		{"cndoom", "CnDoom (Doom)"},
cnheretic		{"cnheretic", "CnDoom (Heretic)"},
cnhexen		{"cnhexen", "CnDoom (Hexen)"},
cnstrife		{"cnstrife", "CnDoom (Strife)"},
cnserver		{"cnserver", "CnDoom (Server)"},
zdaemon		{"zdaemon", "ZDaemon"},
zserv32		{"zserv32", "ZDaemon (Server)"},
prboom-plus		{"prboom-plus", "PrBoom+"},
prboom-plus_server		{"prboom-plus_server", "PrBoom+ (Server)"},
glboom-plus		{"glboom-plus", "PrBoom+ (OpenGL)"},
zandronum		{"zandronum", "Zandronum"},
risen3d		{"risen3d", "Risen3D"},
odamex		{"odamex", "Odamex"},
odasrv		{"odasrv", "Odamex (Server)"},
edge		{"edge", "EDGE"},
vavoom		{"vavoom", "Vavoom"},
vavoom-dedicated		{"vavoom-dedicated", "Vavoom (Server)"},
eternity		{"eternity", "Eternity Engine"},
doomlegacy		{"doomlegacy", "Doom Legacy"},
remood		{"remood", "ReMooD"},
doomretro		{"doomretro", "Doom Retro"},
prboom		{"prboom", "PrBoom"},
prboom_server		{"prboom_server", "PrBoom (Server)"},
glboom		{"glboom", "PrBoom (OpenGL)"},
boom		{"boom", "Boom"},
doomcl		{"doomcl", "csDoom"},
crispy-doom		{"crispy-doom", "Crispy Doom"},
crispy-server		{"crispy-server", "Crispy Doom (Server)"},
jdoom		{"jdoom", "jDoom (Doom)"},
jheretic		{"jheretic", "jDoom (Heretic)"},
jhexen		{"jhexen", "jDoom (Hexen)"},
wdmp		{"wdmp", "WDMP"},
wdmp32s		{"wdmp32s", "WDMP (Win32s)"},
lxdoom		{"lxdoom", "LxDoom"},
lxdoom-game-server		{"lxdoom-game-server", "LxDoom (Server)"},
zdoomgl		{"zdoomgl", "ZDoomGL"},
mochadoom		{"mochadoom", "Mocha Doom"},
mochadoom7		{"mochadoom7", "Mocha Doom (Win 7)"},
strawberry-doom		{"strawberry-doom", "Strawberry Doom"},
strawberry-server		{"strawberry-server", "Strawberry Doom (Server)"},
gldoom		{"gldoom", "GLDoom"},
doom3d		{"doom3d", "Doom3D"},
smmu		{"smmu", "Smack My Marine Up (SMMU)"},
doomplus		{"doomplus", "Doom Plus"},
zdaemongl		{"zdaemongl", "ZDaemonGL"},
doomgl		{"doomgl", "DoomGL"},
dosdoom		{"dosdoom", "DOSDoom"},
tasdoom		{"tasdoom", "TASDoom"},
mbf		{"mbf", "Marine's Best Friend (MBF)"},
linuxxdoom		{"linuxxdoom", "Linux Doom (X)"},
linuxsdoom		{"linuxsdoom", "Linux Doom (SVGAlib)"},

**********************************************************/
