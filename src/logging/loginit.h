//
// Created by aarons on 07.03.23.
//

#ifndef GAFFA_LOGINIT_H
#define GAFFA_LOGINIT_H

#endif //GAFFA_LOGINIT_H

#include "easylogging++.h"
#ifdef ELPP_SYSLOG
#define GAFFALOG(LEVEL) SYSLOG(LEVEL)
#else
#define GAFFALOG(LEVEL) LOG(LEVEL)
#endif