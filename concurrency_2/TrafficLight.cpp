#include "TrafficLight.h"
#include <iostream>

TrafficLight::TrafficLight()
{
    color =LightColor::RED;
    running = true;
}

TrafficLight::~TrafficLight()
{
    {
        std::lock_guard<std:: mutex> lock(mtx);
        running = false;
    }
    cv.notify_all();
    if (lightFuture.valid()){
        lightFuture.wait();
    }
}

void TrafficLight::start()
{
    {
        std::lock_guard<std:: mutex> lock(mtx);
        running = true;
    }
    lightFuture = std::async(&TrafficLight::run, this);
}

LightColor TrafficLight::getColor()
{
    std::lock_guard<std:: mutex> lock(mtx);
    return color;
}

void TrafficLight::waitForGreen()
{
    std::unique_lock<std:: mutex> lock(mtx);
    while(color != LightColor::GREEN){
        cv.wait(lock);
    }

}

void TrafficLight::run()
{
    while(true){
        std::this_thread::sleep_for(std::chrono::seconds(1));
        {
            std::lock_guard<std:: mutex> lock(mtx);
            if (!running) break;

            switch(color){
                case LightColor::RED: color = LightColor::GREEN; break;
                case LightColor::GREEN: color = LightColor::YELLOW; break;
                case LightColor::YELLOW: color = LightColor::RED; break;
            }
        }

        cv.notify_all();

    }
}
