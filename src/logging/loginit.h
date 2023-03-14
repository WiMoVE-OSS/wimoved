//
// Created by aarons on 07.03.23.
//

#ifndef WM_LOGINIT_H
#define WM_LOGINIT_H

#endif //WM_LOGINIT_H

#include "easylogging++.h"
#ifdef ELPP_SYSLOG
#define WMLOG(LEVEL) SYSLOG(LEVEL)
#else
#define WMLOG(LEVEL) LOG(LEVEL)
#endif