
#include "ZDLConfigurationEvents.h"

void ZDLConfigurationEvents::signalSaveConfiguration(ZDLConf *conf){
        emit saveConfiguration(conf);
}
void ZDLConfigurationEvents::signalReadConfiguration(ZDLConf *conf){
        emit readConfiguration(conf);
}

