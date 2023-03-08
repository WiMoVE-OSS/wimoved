#ifndef GAFFA_METRICSMANAGER_H
#define GAFFA_METRICSMANAGER_H

#include <prometheus/exposer.h>
#include <prometheus/registry.h>

#include <memory>

class MetricsManager {
   public:
    MetricsManager();
    MetricsManager(MetricsManager const&) = delete;
    void operator=(MetricsManager const&) = delete;

    static MetricsManager& get_instance();
    prometheus::Counter& get_hostapd_counter_for_type(const std::string& type);
    prometheus::Counter& get_netlink_counter_received();
    prometheus::Counter& get_netlink_counter_processed();

   private:
    prometheus::Exposer exposer;
    prometheus::Family<prometheus::Counter>& hostapd_counter;
    prometheus::Family<prometheus::Counter>& netlink_counter;
};

#endif  // GAFFA_METRICSMANAGER_H
