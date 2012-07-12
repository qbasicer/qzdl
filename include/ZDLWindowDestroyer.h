#ifndef _ZDLWINDOWDESTROYER_H_
#define _ZDLWINDOWDESTROYER_H_

#include "QtGui"
#include "ZDLMainWindow.h"

extern ZDLMainWindow *mw;

class ZDLWindowDestroyer : public QObject {
        Q_OBJECT
        public slots:
                void kill(){
                        delete mw;
                        delete this;
                }
};



#endif
