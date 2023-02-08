//
// Created by richard on 08.02.23.
//

#ifndef GAFFA_TIMEOUTEXCEPTION_H
#define GAFFA_TIMEOUTEXCEPTION_H


#include <exception>
#include <string>
#include <stdexcept>

namespace ipc {
    class TimeoutException : public std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}


#endif //GAFFA_TIMEOUTEXCEPTION_H
