#ifndef WIMOVED_METRICSMANAGER_H
#define WIMOVED_METRICSMANAGER_H

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
    prometheus::Gauge& get_station_gauge();
    prometheus::Gauge& get_vni_gauge();
    prometheus::Histogram& get_event_histogram();

   private:
    prometheus::Exposer exposer;
    prometheus::Family<prometheus::Counter>& hostapd_counter;
    prometheus::Family<prometheus::Gauge>& connection_gauge;
    prometheus::Family<prometheus::Histogram>& histogram;
};

#endif  // WIMOVED_METRICSMANAGER_H
