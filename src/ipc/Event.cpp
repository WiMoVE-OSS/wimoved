#include "Event.h"

#include <utility>

ipc::Event::Event(Station station)
    : station(std::move(station)), created_at(std::chrono::high_resolution_clock::now()) {}
uint64_t ipc::Event::finished_processing() {
    std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(now - created_at).count();
}
