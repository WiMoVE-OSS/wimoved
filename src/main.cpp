#include <csignal>
#include <future>
#include <iostream>
#include <thread>

#include "BridgePerVxlanRenderer.h"
#include "ConfigParser.h"
#include "EventLoop.h"
#include "ipc/Subscriber.h"
#include "logging/loginit.h"
#include "nl/Subscriber.h"

INITIALIZE_EASYLOGGINGPP
#include "Configuration.h"
#include "logging/loginit.h"

std::vector<std::promise<void>> promises(5);
bool promises_resolved = false;
std::mutex promises_mutex;

void handle_signal(int signal) {
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
        GAFFALOG(ERROR) << "An Exception was thrown that was not caught.";
        try {
            std::rethrow_exception(std::current_exception());
        } catch (const std::exception& ex) {
            GAFFALOG(FATAL) << typeid(ex).name() << ": " << ex.what();
        } catch (...) {
            GAFFALOG(FATAL) << "Error while handling exception: " << typeid(std::current_exception()).name();
        }
        GAFFALOG(ERROR) << "errno: " << errno << ": " << std::strerror(errno) << std::endl;
        std::abort();
    });
    el::Configurations defaultConf;
    defaultConf.setToDefault();
    defaultConf.setGlobally(el::ConfigurationType::Format, "[%level] %datetime %fbase:%line - %msg");

#ifdef ELPP_SYSLOG
    el::SysLogInitializer elSyslogInit("Gaffa", LOG_PID | LOG_CONS | LOG_PERROR, LOG_USER);
    el::Loggers::reconfigureLogger("syslog", defaultConf);
#else
    defaultConf.setGlobally(el::ConfigurationType::Filename, Configuration::get_instance().log_path);
#endif

    el::Loggers::reconfigureLogger("default", defaultConf);
}

int main(int argc, char* argv[]) {
    std::string config_path = "/etc/gaffa/config";
    if (argc >= 2) {
        config_path = argv[1];
    }
    GAFFALOG(INFO) << "Gaffa is starting";
    ConfigParser bla(config_path);
    setup_logger();

    std::signal(SIGINT, handle_signal);
    std::signal(SIGTERM, handle_signal);

    SynchronizedQueue<ipc::Event> ipc_queue;
    SynchronizedQueue<nl::Event> nl_queue;
    BridgePerVxlanRenderer renderer;

    std::chrono::duration timeout = std::chrono::seconds(1);
    EventLoop event_loop(renderer, ipc_queue, nl_queue);

    std::thread ipc_subscriber_thread(
        [&ipc_queue, &timeout]() { ipc::Subscriber(ipc_queue, timeout).loop(promises[0].get_future()); });
    std::thread nl_subscriber_thread(
        [&nl_queue, &timeout]() { nl::Subscriber(nl_queue, timeout).loop(promises[1].get_future()); });
    std::thread event_loop_ipc_thread([&event_loop]() { event_loop.loop_ipc_queue(promises[2].get_future()); });
    std::thread event_loop_nl_thread([&event_loop]() { event_loop.loop_nl_queue(promises[3].get_future()); });
    std::thread cleanup_thread([&renderer]() {
        std::future<void> future = promises[4].get_future();
        ipc::Caller caller;
        while (future.wait_for(std::chrono::seconds(0)) != std::future_status::ready) {
            std::this_thread::sleep_for(std::chrono::seconds(Configuration::get_instance().cleanup_interval));
            renderer.cleanup([&caller]() { return caller.connected_stations(); });
        }
    });
    ipc_subscriber_thread.join();
    nl_subscriber_thread.join();
    event_loop_ipc_thread.join();
    event_loop_nl_thread.join();
    cleanup_thread.join();
}
