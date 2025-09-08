

#include <mutex>

enum class LightColor {RED, GREEN, YELLOW};

class TrafficLight {
public:
    void run(); //Runs in a thread: cycles Lights
    LightColor getColor();
    void waitForGreen(); // blocks until green

private:
    LightColor color;
    std::mutex mtx;
    std::condition_variable cv;

};