/*
 * This file is part of qZDL
 * Copyright (C) 2007-2010  Cody Harris
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

#define ZDL_VERSION_STRING	"3.2.2.3 beta"

extern QDebug *zdlDebug;


#define LOGDATA() (*zdlDebug)
#define LOGDATAO() (*zdlDebug)
#define DPTR(ptr) QString("")

#if QT_VERSION < 0x94040
#define LOCK_CLASS		QMutex
#define LOCK_BUILDER()		new QMutex(QMutex::Recursive)
#define GET_READLOCK(mlock)	(mlock)->lock()
#define RELEASE_READLOCK(mlock)	(mlock)->unlock()
#define GET_WRITELOCK(mlock)	(mlock)->lock()
#define RELEASE_WRITELOCK(mlock)	(mlock)->unlock()
#define TRY_READLOCK(mlock, to)	(mlock)->tryLock(to)
#define TRY_WRITELOCK(mlock, to) (mlock)->tryLock(to)
#warning Using Old Locking

#else

#define LOCK_CLASS              QReadWriteLock
#define LOCK_BUILDER()          new QReadWriteLock(QReadWriteLock::Recursive)
#define GET_READLOCK(lock)      (lock)->lockForRead()
#define RELEASE_READLOCK(lock)  (lock)->unlock()
#define GET_WRITELOCK(lock)     (lock)->lockForWrite()
#define RELEASE_WRITELOCK(lock) (lock)->unlock()
#define TRY_READLOCK(lock, to)	(lock)->tryLockForRead(to)
#define TRY_WRITELOCK(lock, to) (lock)->tryLockForWrite(to)
#warning Using New Locking

#endif	


#include "zdlline.hpp"
#include "zdlsection.hpp"
#include "zdlconf.hpp"

#endif
