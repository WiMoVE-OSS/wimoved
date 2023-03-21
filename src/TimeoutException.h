#ifndef WIMOVED_TIMEOUTEXCEPTION_H
#define WIMOVED_TIMEOUTEXCEPTION_H

#include <exception>
#include <stdexcept>
#include <string>

class TimeoutException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif  // WIMOVED_TIMEOUTEXCEPTION_H
