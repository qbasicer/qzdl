#include <QVector>
#include <QFile>
#include "ZDLMainWindow.h"
#include "ZDLAdvancedMultiplayerDialog.h"
#include <QtTest/QtTest>
#include <QCoreApplication>
#include "libwad.h"
#include "ZDLConfigurationManager.h"
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

QTEST_MAIN(TestQZDL)
#include "tst_qzdl.moc"
