#ifndef WIMOVED_LOGINIT_H
#define WIMOVED_LOGINIT_H

#endif //WIMOVED_LOGINIT_H

#include "easylogging++.h"
#ifdef ELPP_SYSLOG
#define WMLOG(LEVEL) SYSLOG(LEVEL)
#else
#define WMLOG(LEVEL) LOG(LEVEL)
#endif