#include <QIODevice>
#include <QSettings>
#include "ini.h"

int handler(void*, const char*, const char*, const char*);
char* reader(char* str, int num, void* stream);

bool readZDLConf(QIODevice &, QSettings::SettingsMap &);
bool writeZDLConf(QIODevice &, const QSettings::SettingsMap &);

void keySort(QStringList &);

bool sectionComparator(QString keyA, QString keyB);
bool fileComparator(QString keyA, QString keyB);
bool pairComparator(QString keyA, QString keyB);
bool pairIndexComparator(QString keyA, QString keyB);

class ZDLConfigurationManager
{
public:
	static void setActiveConfiguration(QSettings *);
	static QSettings *getActiveConfiguration();
private:
	static QSettings *settings;
};
