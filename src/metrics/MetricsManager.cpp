#include "MetricsManager.h"

#include <prometheus/counter.h>

static std::shared_ptr<prometheus::Registry> registry = std::make_shared<prometheus::Registry>();

MetricsManager::MetricsManager()
    : exposer("0.0.0.0:9500"),
      hostapd_counter(prometheus::BuildCounter()
                          .Name("hostapd_events")
                          .Help("Number of received hostapd events")
                          .Register(*registry)),
      netlink_counter(prometheus::BuildCounter()
                          .Name("netlink_events")
                          .Help("Number of received netlink events")
                          .Register(*registry)) {
    exposer.RegisterCollectable(registry);
}

MetricsManager &MetricsManager::get_instance() {
    static MetricsManager instance;
    return instance;
}

prometheus::Counter &MetricsManager::get_hostapd_counter_for_type(const std::string &type) {
    return hostapd_counter.Add({{"type", type}});
}

prometheus::Counter &MetricsManager::get_netlink_counter_processed() {
    return netlink_counter.Add({{"type", "processed"}});
}

prometheus::Counter &MetricsManager::get_netlink_counter_received() {
    return netlink_counter.Add({{"type", "received"}});
}
