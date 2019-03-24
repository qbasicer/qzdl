#include <algorithm>
#include <QBuffer>
#include <QVector>
#include <QFile>
#include "ZDLMainWindow.h"
#include "ZDLAdvancedMultiplayerDialog.h"
#include <QtTest/QtTest>
#include <QCoreApplication>
#include "libwad.h"
#include "ZDLConfigurationManager.h"
#include "confparser.h"
#include "ZDLNullDevice.h"

#include <QDebug>

QDebug *zdlDebug;
ZDLMainWindow *mw;

class TestQZDL: public QObject
{
	Q_OBJECT
private slots:
	void test_gui_readINI();
	void test_libwad_getMapNames();
	void test_keyComparator();
	void test_readZDLConf();
private:
	QString resource(QString);
};

void TestQZDL::test_gui_readINI()
{
	// This is a blackbox GUI integration test. It reads a configuration file and checks for
	// its values in the UI.

	// Has side effects, due to qZDL's use of global and static data.

	ZDLNullDevice nullDev;
	zdlDebug = new QDebug(&nullDev);
	ZDLConfigurationManager::setConfigFileName(resource("qZDL.ini"));
	ZDLConfigurationManager::setWhy(ZDLConfigurationManager::USER_SPECIFIED);
	ZDLConf tconf;
	tconf.readINI(resource("qZDL.ini"));
	ZDLConfigurationManager::setActiveConfiguration(&tconf);


	auto iniFormat = QSettings::registerFormat("ini", readZDLConf, writeZDLConf);
	QSettings testSettings(resource("qZDL2.ini"), iniFormat);
	ZDLSettingsManager::setInstance(&testSettings);

	mw = new ZDLMainWindow();
	mw->startRead();
	QCOMPARE(mw->getExecutable(), "/usr/bin/gzdoom");

	QStringList args {
		"-iwad", "/home/user/doom/DOOM2.WAD",
		"-skill", "3",
		"+map", "MAP02",
		"+set", "dmflags", "1",
		"+set", "dmflags2", "1",
		"-file",
		"/usr/share/games/doom/lights.pk3",
		"/usr/share/games/doom/brightmaps.pk3",
		"/home/user/doom/sunlust/sunlust.wad",
		"-deathmath",
		"-host", "2",
		"+set", "fraglimit", "3",
		"-port", "81",
		"-extratic",
		"-netmode", "1",
		"-dup", "1",
		"-nomonsters -turbo 250",
		"-playdemo speedrun.lmp"
	};
	qDebug() << mw->getArguments();
	QCOMPARE(mw->getArguments(), args);
	QCOMPARE(mw->getExtraArgs(), "-playdemo speedrun.lmp");
	QCOMPARE(mw->getMode(), "Deathmatch");
	QCOMPARE(mw->getHostAddy(), "192.168.1.10");
	QCOMPARE(mw->getPlayers(), "2");
	QCOMPARE(mw->getFragLmit(), "3");
	QCOMPARE(mw->getDMFlags(), "1");
	QCOMPARE(mw->getDMFlags2(), "1");

	QCOMPARE(mw->getAlwaysArgs(), "-nomonsters -turbo 250");
	QCOMPARE(mw->getLaunchClose(), Qt::Checked);
	QCOMPARE(mw->getShowPaths(), Qt::Checked);
	QCOMPARE(mw->getLaunchZDL(), Qt::Checked);
	QCOMPARE(mw->getSavePaths(), Qt::Checked);

	auto zampd = new ZDLAdvancedMultiplayerDialog();
	QCOMPARE(Qt::Checked, zampd->getEnable());
	QCOMPARE(Qt::Checked, zampd->getExtraTic());
	QCOMPARE(zampd->getNetMode(), "1 (Client/Server Model)");
	QCOMPARE(zampd->getPortNo(), "81");
	QCOMPARE("1", zampd->getDupMode());

	delete mw;
	delete zampd;
}

QString TestQZDL::resource(QString file)
{
	return QCoreApplication::applicationDirPath() + "/resources/" + file;
}

void TestQZDL::test_libwad_getMapNames()
{
	DoomWad wad(resource("dummy.wad"));
	wad.open();
	auto maps = wad.getMapNames();
	QCOMPARE(maps[0], QString{"MAP01"});
}

void TestQZDL::test_keyComparator()
{
	QStringList shuffledKeys
	{
		"zdl.save/gametype",
		"zdl.general/zdllaunch",
		"zdl.general/autoclose",
		"zdl.general/dmflags",
		"zdl.net/extratic",
		"zdl.save/dlgmode",
		"zdl.save/file1",
		"zdl.save/iwad",
		"zdl.net/port",
		"zdl.general/engine",
		"zdl.save/fraglimit",
		"zdl.general/alwaysadd",
		"zdl.save/host",
		"zdl.save/file2",
		"zdl.general/lastDir",
		"zdl.general/dmflags2",
		"zdl.general/showpaths",
		"zdl.net/advenabled",
		"zdl.iwads/i1n",
		"zdl.save/extra",
		"zdl.general/version",
		"zdl.ports/p0f",
		"zdl.save/dmflags2",
		"zdl.save/port",
		"zdl.net/dup",
		"zdl.net/netmode",
		"zdl.ports/p0n",
		"zdl.ports/p1n",
		"zdl.ports/p1f",
		"zdl.general/conflib",
		"zdl.save/warp",
		"zdl.general/fraglimit",
		"zdl.save/players",
		"zdl.iwads/i0f",
		"zdl.iwads/i0n",
		"zdl.save/dmflags",
		"zdl.general/rememberFilelist",
		"zdl.save/file0",
		"zdl.general/host",
		"zdl.general/windowpos",
		"zdl.save/skill",
		"zdl.iwads/i1f",
		"zdl.general/windowsize"
	};


	QStringList sortedKeys{shuffledKeys};
	keySort(sortedKeys);

	// This is adequate. Grouped by section, with IWADs, PWADs and source ports in order.
	QStringList expected {
		"zdl.general/zdllaunch",
		"zdl.general/autoclose",
		"zdl.general/dmflags",
		"zdl.general/engine",
		"zdl.general/alwaysadd",
		"zdl.general/lastDir",
		"zdl.general/dmflags2",
		"zdl.general/showpaths",
		"zdl.general/version",
		"zdl.general/conflib",
		"zdl.general/fraglimit",
		"zdl.general/rememberFilelist",
		"zdl.general/host",
		"zdl.general/windowpos",
		"zdl.general/windowsize",
		"zdl.iwads/i0n",
		"zdl.iwads/i0f",
		"zdl.iwads/i1n",
		"zdl.iwads/i1f",
		"zdl.net/extratic",
		"zdl.net/port",
		"zdl.net/advenabled",
		"zdl.net/dup",
		"zdl.net/netmode",
		"zdl.ports/p0n",
		"zdl.ports/p0f",
		"zdl.ports/p1n",
		"zdl.ports/p1f",
		"zdl.save/gametype",
		"zdl.save/dlgmode",
		"zdl.save/iwad",
		"zdl.save/fraglimit",
		"zdl.save/host",
		"zdl.save/extra",
		"zdl.save/dmflags2",
		"zdl.save/port",
		"zdl.save/warp",
		"zdl.save/players",
		"zdl.save/dmflags",
		"zdl.save/file0",
		"zdl.save/skill",
		"zdl.save/file1",
		"zdl.save/file2"
	};


	QCOMPARE(sortedKeys, expected);
}

void TestQZDL::test_readZDLConf()
{
	QString ini {
		"[zdl.general]\n"
		"autoclose=1\n"
		"[zdl.save]\n"
		"gametype=2\n"
	};
	QSettings::SettingsMap map;
	QByteArray iniBytes{ini.toUtf8()};
	QBuffer buffer{&iniBytes};
	QVERIFY(buffer.open(QIODevice::ReadOnly));
	readZDLConf(buffer, map);
	QStringList keys{
		"zdl.general/autoclose",
		"zdl.save/gametype"
	};
	QCOMPARE(map.keys(), keys);
	QCOMPARE(map["zdl.general/autoclose"], "1");
	QCOMPARE(map["zdl.save/gametype"], "2");
}

QTEST_MAIN(TestQZDL)
#include "tst_qzdl.moc"
