#ifndef GAFFA_VLANMISSINGEXCEPTION_H
#define GAFFA_VLANMISSINGEXCEPTION_H


#include <stdexcept>

class VlanMissingException : public std::runtime_error {
    using std::runtime_error::runtime_error;
};


#endif //GAFFA_VLANMISSINGEXCEPTION_H
