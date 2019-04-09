
#include "ZDLConfigurationEvents.h"

void ZDLConfigurationEvents::signalNewConfiguration(ZDLConf *conf){
		emit newConfiguration(conf);
}
void ZDLConfigurationEvents::signalSaveConfiguration(ZDLConf *conf){
		emit saveConfiguration(conf);
}
void ZDLConfigurationEvents::signalReadConfiguration(ZDLConf *conf){
		emit readConfiguration(conf);
}

