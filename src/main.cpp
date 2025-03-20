#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <utility>
#include <spdlog/spdlog.h>
#include <pubsub/pubsub.hpp>

int main() {
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [thread %t] %v");
    spdlog::info("main.cpp");

    // Temperature events (time_t, float)
    EventBus<std::string, float> temperature_bus;

    auto put_temp = [](const Message<std::string, float>& msg) {
        spdlog::info("put callback: {} {}{}", msg.first, msg.second, "°C");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    };
    
    temperature_bus.subscribe(put_temp);

    float value = 24.237f;
    for (int i = 0; i < 10; i++) {
        std::string key = std::to_string(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now())) + ".location";
        temperature_bus.publish(key, value);
        spdlog::info("{} before publish: {} {}", i+1, key, value);
        spdlog::info("{} after publish : {} {}", i+1, key, value);

        value += 0.23f;

        std::this_thread::sleep_for(std::chrono::seconds(2));
    }


    // String events (string, string)
    EventBus<std::string, std::string> log_bus;
    
    log_bus.subscribe([](const Message<std::string, std::string>& msg) {
        std::cout << "log thread id: " << std::this_thread::get_id() << std::endl;
        std::cout << "Log [" << msg.first << "]: " << msg.second << "\n";
    });

    log_bus.publish("INFO", "System started");

    // Position events (int, std::pair<double, double>)
    EventBus<int, std::pair<double, double>> position_bus;
    
    position_bus.subscribe([](const Message<int, std::pair<double, double>>& msg) {
        std::cout << "position thread id: " << std::this_thread::get_id() << std::endl;
        std::cout << "Object " << msg.first 
                  << " moved to (" << msg.second.first 
                  << ", " << msg.second.second << ")\n";
    });

    position_bus.publish(1, std::make_pair(23.4, 45.6));

    // Sleep to allow processing
    std::this_thread::sleep_for(std::chrono::seconds(1));

    return 0;
}
