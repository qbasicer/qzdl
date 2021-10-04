/* This is put into it's own header file as I couldn't figure out a better
 * way to include it both in ZDLFileList.cpp and ZDLMainWindow.cpp
 */
#ifndef _DISABLED_H_
#define _DISABLED_H_

#include <QString>
#include <QStringList>

const QString disabledKey = "zdl.save/disabled";

/* Due to the way the configuration is implemented I'm pretty
 * sure this is more efficient than creating a set from the
 * disabled value every time we want to do something with it.
 */
inline bool disabledScan(QString str, int val) {
        // 0x050C08 is the Qt version number (5.12.8) in hexadecimal. This is
        // the Qt version available in Kubuntu 20.04. It comes from
        // qtcoreversion.h
#if QTCORE_VERSION <= 0x050C08
        for (auto s : str.split(",", QString::SkipEmptyParts))
#else
        for (auto s : str.split(",", Qt::SkipEmptyParts))
#endif
	{
                bool ok;

                if ((val == s.toInt(&ok, 10)) && ok)
		{
                        return true;
                }
        }

        return false;
}

#endif
