#include <chrono>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <thread>
#include <vector>
#include <atomic>
#include <utility>
#include <spdlog/spdlog.h>

template<typename T1, typename T2>
struct Message {
    T1 first;
    T2 second;
};

template<typename T1, typename T2>
class EventBus {
public:
    using MessageType = Message<T1, T2>;
    using Subscriber = std::function<void(const MessageType&)>;

    EventBus() : running_(true) {
        spdlog::info("EventBus constructor");
        dispatch_thread_ = std::thread([this] { dispatchLoop(); });
    }

    ~EventBus() {
        spdlog::info("EventBus destructor");
        running_ = false;
        cv_.notify_all();
        if (dispatch_thread_.joinable()) {
            dispatch_thread_.join();
        }
    }

    // Publisher interface
    void publish(T1 first, T2 second) {
        MessageType msg{
            .first = std::move(first),
            .second = std::move(second)
        };

        {
            std::lock_guard<std::mutex> lock(mutex_);
            message_queue_.push(msg);
            spdlog::info("publish, queue size: {}", message_queue_.size());
        }
        cv_.notify_one();
    }

    // Subscriber interface
    void subscribe(Subscriber callback) {
        std::lock_guard<std::mutex> lock(mutex_);
        subscribers_.push_back(std::move(callback));
        spdlog::info("subscriber size: {}", subscribers_.size());
    }

private:
    void dispatchLoop() {
        while (running_) {
            std::unique_lock<std::mutex> lock(mutex_);
            
            cv_.wait(lock, [this] { 
                return !message_queue_.empty() || !running_; 
            });

            while (!message_queue_.empty() && running_) {
                auto msg = message_queue_.front();
                spdlog::info("message loop, queue size: {}", message_queue_.size());
                message_queue_.pop();
                
                auto subs = subscribers_;
                lock.unlock();

                for (const auto& subscriber : subs) {
                    subscriber(msg);
                }

                lock.lock();
            }
        }
    }

    std::queue<MessageType> message_queue_;
    std::vector<Subscriber> subscribers_;
    std::mutex mutex_;
    std::condition_variable cv_;
    std::atomic<bool> running_;
    std::thread dispatch_thread_;
};

// Helper function to create a message
template<typename T1, typename T2>
Message<T1, T2> make_message(T1 first, T2 second) {
    return Message<T1, T2>{std::move(first), std::move(second)};
}

// Helper function to publish with current timestamp
template<typename T>
void publish_with_timestamp(EventBus<std::time_t, T>& bus, T value) {
    bus.publish(
        std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()),
        std::move(value)
    );
}
