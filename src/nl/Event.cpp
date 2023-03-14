#include "Event.h"

#include <utility>

nl::Event::Event(std::string interface_name, uint32_t ifindex) : interface_name(std::move(interface_name)), ifindex(ifindex) {

}