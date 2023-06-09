#include "MetricsManager.h"

#include <prometheus/counter.h>
#include <prometheus/histogram.h>

const static std::shared_ptr<prometheus::Registry> REGISTRY = std::make_shared<prometheus::Registry>();
const static auto BUCKET_BOUNDARIES = prometheus::Histogram::BucketBoundaries{
    30000, 35000, 40000, 45000, 50000, 55000, 60000, 65000, 70000, 75000, 80000, 85000, 90000, 95000, 100000};

MetricsManager::MetricsManager()
    : exposer("0.0.0.0:9500"),
      hostapd_counter(prometheus::BuildCounter()
                          .Name("hostapd_events")
                          .Help("Number of received hostapd events")
                          .Register(*REGISTRY)),
      connection_gauge(
          prometheus::BuildGauge().Name("connections").Help("Number of connections to the AP").Register(*REGISTRY)),
      histogram(prometheus::BuildHistogram()
                    .Name("processing_time")
                    .Help("Time needed from event to completely set up station")
                    .Register(*REGISTRY)) {
    exposer.RegisterCollectable(REGISTRY);
}

MetricsManager &MetricsManager::get_instance() {
    static MetricsManager instance;
    return instance;
}

prometheus::Counter &MetricsManager::get_hostapd_counter_for_type(const std::string &type) {
    return hostapd_counter.Add({{"type", type}});
}

prometheus::Gauge &MetricsManager::get_station_gauge() { return connection_gauge.Add({{"type", "stations"}}); }

prometheus::Gauge &MetricsManager::get_vni_gauge() { return connection_gauge.Add({{"type", "vnis"}}); }

prometheus::Histogram &MetricsManager::get_event_histogram() {
    return histogram.Add({{"type", "connect"}}, BUCKET_BOUNDARIES);
}
