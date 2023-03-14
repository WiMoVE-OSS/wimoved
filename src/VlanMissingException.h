#ifndef WM_VLANMISSINGEXCEPTION_H
#define WM_VLANMISSINGEXCEPTION_H

#include <stdexcept>

class VlanMissingException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};

#endif  // WM_VLANMISSINGEXCEPTION_H
