#include "Event.h"

#include <utility>

ipc::Event::Event(Station station) : station(std::move(station)) {

}