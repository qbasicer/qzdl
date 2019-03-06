/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
 * Copyright (C) 2018-2019  Lcferrum
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
 
#ifndef _ZDLCOMMON_H_
#define _ZDLCOMMON_H_
#include <QtCore>
using namespace std;

#define ZDL_FLAG_NAMELESS	0x00001

#define ZDL_VERSION_STRING "3-1.1"
#define ZDL_DEV_BUILD 1
#define ZDL_PRIVATE_VERSION_STRING "3.2.2.3~2017.01.11.git.6e5fe853c7-1.1+lcferrum"

extern QDebug *zdlDebug;

#if defined(ZDL_BLACKBOX)
#include <QtCore>
#define LOGDATA() (*zdlDebug) << (QDateTime::currentDateTime().toString("[yyyy:MM:dd/hh:mm:ss.zzz]").append("@").append(__PRETTY_FUNCTION__).append("@").append(__FILE__).append(":").append(QString::number(__LINE__)).append("\t"))
#define LOGDATAO() (*zdlDebug) << (QDateTime::currentDateTime().toString("[yyyy:MM:dd/hh:mm:ss.zzz]").append("@").append(__PRETTY_FUNCTION__).append("@").append(__FILE__).append(":").append(QString::number(__LINE__)).append("#this=").append(DPTR(this)).append("\t"))

#if !defined(Q_WS_MAC)

#if UINTPTR_MAX == 0xffffffff
#ifndef _ZDL_NO_WARNINGS
#ifdef __GNUC__
#warning 32bit
#else
#pragma message("Warning: 32bit")
#endif
#endif
#define DPTR(ptr) QString("0x").append(QString("%1").arg((qulong)ptr, 0, 8, 16)
#else
#define DPTR(ptr) QString("0x").append(QString::number((qulonglong)ptr,16))
#endif

#else

#define DPTR(ptr) QString("0x").append("PTR")

#endif

#else
#define LOGDATA() (*zdlDebug)
#define LOGDATAO() (*zdlDebug)
#define DPTR(ptr) QString("")
#endif


#if defined(Q_WS_WIN)
#define ASSOCIATE_FILETYPES_AVAILBLE
extern void RegisterFileType(char *ext,char *type,char *nicetype,char *exe,char* command,int icon);
extern void RegisterFileTypeQt(QString extension, QString type, QString niceType, QString exec, QString command, int iconIndex);
#define ASSOCIATE_FILETYPES() {\
		QString rawExecutablePath = ZDLConfigurationManager::getExec().replace("/","\\");\\
		RegisterFileTypeQt(".zdl", "ZDL.SaveFile", "ZDL Saved Configuration File", rawExecutablePath, "\"%1\"", 1);\
	}
#else
//Do nothing
#define ASSOCIATE_FILETYPES() {}
#endif

#if QT_VERSION < 0x94040
#define LOCK_CLASS		QMutex
#define LOCK_BUILDER()		new QMutex(QMutex::Recursive)
#define GET_READLOCK(mlock)	(mlock)->lock()
#define RELEASE_READLOCK(mlock)	(mlock)->unlock()
#define GET_WRITELOCK(mlock)	(mlock)->lock()
#define RELEASE_WRITELOCK(mlock)	(mlock)->unlock()
#define TRY_READLOCK(mlock, to)	(mlock)->tryLock(to)
#define TRY_WRITELOCK(mlock, to) (mlock)->tryLock(to)
#ifndef _ZDL_NO_WARNINGS
#ifdef __GNUC__
#warning Using Old Locking
#else
#pragma message("Warning: Using Old Locking")
#endif
#endif

#else

#define LOCK_CLASS              QReadWriteLock
#define LOCK_BUILDER()          new QReadWriteLock(QReadWriteLock::Recursive)
#define GET_READLOCK(lock)      (lock)->lockForRead()
#define RELEASE_READLOCK(lock)  (lock)->unlock()
#define GET_WRITELOCK(lock)     (lock)->lockForWrite()
#define RELEASE_WRITELOCK(lock) (lock)->unlock()
#define TRY_READLOCK(lock, to)	(lock)->tryLockForRead(to)
#define TRY_WRITELOCK(lock, to) (lock)->tryLockForWrite(to)
#ifndef _ZDL_NO_WARNINGS
#ifdef __GNUC__
#warning Using New Locking
#else
#pragma message("Warning: Using New Locking")
#endif
#endif

#endif	


#include "zdlline.hpp"
#include "zdlsection.hpp"
#include "zdlconf.hpp"
#include "zdlvariables.hpp"

#endif
