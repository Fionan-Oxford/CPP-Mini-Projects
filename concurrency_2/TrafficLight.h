#pragma once
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

enum class LightColor {RED, YELLOW, GREEN};

class TrafficLight{
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
