#include <algorithm>
#include <QIODevice>
#include <QRegularExpression>
#include <QSettings>
#include <QTextStream>
#include "ini.h"
#include "zdlconf.h"

int handler(void* user, const char* section, const char* name, const char* value)
{
	QString qsection{section};
	QString qname{name};
	QString qvalue{value};
	auto map = static_cast<QSettings::SettingsMap *>(user);
	map->insert(qsection + "/" + qname, qvalue);
	return 0;
}


char* reader(char* str, int num, void* stream)
{
	auto io = static_cast<QIODevice *>(stream);

	if (io->readLine(str, num) != -1)
	{
		return str;
	}
	return nullptr;
}


bool readZDLConf(QIODevice &device, QSettings::SettingsMap &map)
{
	if (ini_parse_stream(reader, &device, handler, &map) < 0)
	{
		return false;
	}
	return true;
}


bool writeZDLConf(QIODevice &device, const QSettings::SettingsMap &map)
{
	QStringList keys{map.keys()};
	keySort(keys);
	QString section{""};

	QRegularExpression re{"^(zdl.[a-z]+)/(\\w+)"};
	QTextStream stream{&device};

	for (auto &key: keys)
	{
		auto match = re.match(key);
		auto keySection = match.captured(1);
		if (keySection.isNull())
		{
			return false;
		}
		if (keySection != section)
		{
			section = keySection;		
			stream << "[" << section << "]\n";
		}
		stream << match.captured(2) << "=" << map[key].toString() << "\n";
	}

	return true;
}


void keySort(QStringList &keys)
{
	std::stable_sort(keys.begin(), keys.end(), fileComparator);
	std::stable_sort(keys.begin(), keys.end(), pairComparator);
	std::stable_sort(keys.begin(), keys.end(), pairIndexComparator);
	std::stable_sort(keys.begin(), keys.end(), sectionComparator);
}


bool sectionComparator(QString keyA, QString keyB)
{
	QRegularExpression re{"^zdl.([a-z]+)/"};
	return re.match(keyA).captured(1) < re.match(keyB).captured(1);
}


bool fileComparator(QString keyA, QString keyB)
{
	QRegularExpression re{".file(\\d+)"};
	auto matchA = re.match(keyA);
	int valueA = 0;
	if (matchA.hasMatch())
	{
		valueA = matchA.captured(1).toInt() + 1;
	}
	int valueB = 0;
	auto matchB = re.match(keyB);
	if (matchB.hasMatch())
	{
		valueB = matchB.captured(1).toInt() + 1;
	}
	return valueA < valueB;
}


bool pairComparator(QString keyA, QString keyB)
{
	QRegularExpression re{"[ip]\\d+([nf])"};
	return re.match(keyB).captured(1) < re.match(keyA).captured(1);
}


bool pairIndexComparator(QString keyA, QString keyB)
{
	QRegularExpression re{"[ip](\\d+)[nf]"};
	return re.match(keyA).captured(1) < re.match(keyB).captured(1);
}

QSettings *ZDLConfigurationManager::settings = nullptr;

void ZDLConfigurationManager::setActiveConfiguration(QSettings *instance)
{
	settings = instance;
}

QSettings *ZDLConfigurationManager::getActiveConfiguration()
{
	return settings;
}
