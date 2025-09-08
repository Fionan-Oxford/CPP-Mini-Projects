#pragma once
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

enum class LightColor { RED, GREEN, YELLOW };

class TrafficLight {
public:
    TrafficLight();
    ~TrafficLight();

    void start();
    LightColor getColor();
    void waitForGreen();

private:
    void run();

    LightColor color;
    bool running;
    std::mutex mtx;
    std::condition_variable cv;
    std::future<void> lightFuture;
};
