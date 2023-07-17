#include <csignal>
#include <future>
#include <iostream>
#include <span>
#include <thread>

#include "BridgePerVxlanRenderer.h"
#include "ConfigParser.h"
#include "Configuration.h"
#include "EventLoop.h"
#include "ipc/Subscriber.h"
#include "logging/loginit.h"

// NOLINTBEGIN(cppcoreguidelines-avoid-non-const-global-variables)
std::vector<std::promise<void>> promises(3);
bool promises_resolved = false;
std::mutex promises_mutex;
// NOLINTEND(cppcoreguidelines-avoid-non-const-global-variables)

void handle_signal(int __attribute__((unused)) signal) {
    std::lock_guard g(promises_mutex);
    if (promises_resolved) {
        return;
    }
    promises_resolved = true;
    for (std::promise<void>& promise : promises) {
        promise.set_value();
    }
}

void setup_logger() {
    std::set_terminate([]() -> void {
        WMLOG(ERROR) << "An Exception was thrown that was not caught.";
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& ex) {
            WMLOG(FATAL) << typeid(ex).name() << ": " << ex.what();
        } catch (...) {
            WMLOG(FATAL) << "Error while handling exception error=" << typeid(std::current_exception()).name();
        }
        WMLOG(ERROR) << "errno=" << errno << " error=" << std::strerror(errno) << std::endl;
        std::abort();
    });
    el::Configurations default_configuration;
    default_configuration.setToDefault();
    default_configuration.setGlobally(el::ConfigurationType::Format, "[%level] %datetime %fbase:%line - %msg");

#ifdef ELPP_SYSLOG
    el::SysLogInitializer elSyslogInit("wimoved", LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);
    el::Loggers::reconfigureLogger("syslog", default_configuration);
#else
    default_configuration.setGlobally(el::ConfigurationType::Filename, Configuration::get_instance().log_path);
#endif

    el::Loggers::reconfigureLogger("default", default_configuration);
}

// NOLINTNEXTLINE(bugprone-exception-escape)
int main(int argc, char* argv[]) {
    std::string config_path = "/etc/wimoved/config";
    auto args = std::span(argv, argc);
    if (args.size() >= 2) {
        config_path = args[1];
    }
    WMLOG(INFO) << "Starting wimoved.";
    ConfigParser::from_file(config_path).populate_configuration(Configuration::get_instance());
    setup_logger();

    if (std::signal(SIGINT, handle_signal) == SIG_ERR || std::signal(SIGTERM, handle_signal) == SIG_ERR) {
        WMLOG(FATAL) << "Could not set up signal handlers error=" << std::strerror(errno);
    }

    SynchronizedQueue<ipc::Event> ipc_queue;
    BridgePerVxlanRenderer renderer;

    std::chrono::duration timeout = std::chrono::seconds(1);
    EventLoop event_loop(renderer, ipc_queue);

    std::thread ipc_subscriber_thread(
        [&ipc_queue, &timeout]() { ipc::Subscriber(ipc_queue, timeout).loop(promises[0].get_future()); });
    std::thread event_loop_ipc_thread([&event_loop]() { event_loop.loop_ipc_queue(promises[1].get_future()); });
    std::thread cleanup_thread([&renderer]() {
        std::future<void> future = promises[2].get_future();
        ipc::Caller caller;
        while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            renderer.cleanup([&caller]() { return caller.connected_stations(); });
            std::this_thread::sleep_for(std::chrono::seconds(Configuration::get_instance().cleanup_interval));
        }
    });
    ipc_subscriber_thread.join();
    event_loop_ipc_thread.join();
    cleanup_thread.join();
}
