#include "TrafficLight.h"
#include <iostream>
#include <thread>

int main() {
    TrafficLight light;
    light.start();

    std::thread driver([&light]() {
        std::cout << "[Driver] Waiting for green light...\n";
        light.waitForGreen();
        std::cout << "[Driver] Green light! Proceeding.\n";
    });

    driver.join();
    return 0;
}
