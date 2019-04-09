#ifndef _ZDLCONFIGURATIONEVENTS_HPP_
#define _ZDLCONFIGURATIONEVENTS_HPP_

#include <QtCore>
#include "zdlconf.hpp"
class ZDLConfigurationEvents : public QObject {
		Q_OBJECT
		public:
				void signalNewConfiguration(ZDLConf *zconf);
				void signalReadConfiguration(ZDLConf *zconf);
				void signalSaveConfiguration(ZDLConf *zconf);
		signals:
				void newConfiguration(ZDLConf *zconf);
				void saveConfiguration(ZDLConf *zconf);
				void readConfiguration(ZDLConf *zconf);
};

#endif

