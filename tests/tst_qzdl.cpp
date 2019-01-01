#include <QVector>
#include <QFile>
#include <QtTest/QtTest>
#include <QCoreApplication>
#include "libwad.h"

class TestQZDL: public QObject
{
	Q_OBJECT
private slots:
	void test_libwad_getMapNames();
private:
	QString resource(QString);
};

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
