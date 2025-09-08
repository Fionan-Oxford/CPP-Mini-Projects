#include <iostream>
#include <mutex>
#include <condition_variable>
#include <thread>

class FizzBuzz {
private:
    int n;
    int current;
    std::mutex mtx;
    std::condition_variable cv;

public:
    FizzBuzz(int n) : n(n), current(1) {}

    void fizz() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return current > n || (current % 3 == 0 && current % 5 != 0);
            });
            if (current > n) break;
            std::cout << "fizz\n";
            ++current;
            cv.notify_all();
        }
    }

    void buzz() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return current > n || (current % 5 == 0 && current % 3 != 0);
            });
            if (current > n) break;
            std::cout << "buzz\n";
            ++current;
            cv.notify_all();
        }
    }

    void fizzbuzz() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return current > n || (current % 15 == 0);
            });
            if (current > n) break;
            std::cout << "fizzbuzz\n";
            ++current;
            cv.notify_all();
        }
    }

    void number() {
        while (true) {
            std::unique_lock<std::mutex> lock(mtx);
            cv.wait(lock, [this]() {
                return current > n || (current % 3 != 0 && current % 5 != 0);
            });
            if (current > n) break;
            std::cout << current << "\n";
            ++current;
            cv.notify_all();
        }
    }
};

int main() {
    int n = 15;
    FizzBuzz fb(n);

    std::thread t1(&FizzBuzz::fizz, &fb);
    std::thread t2(&FizzBuzz::buzz, &fb);
    std::thread t3(&FizzBuzz::fizzbuzz, &fb);
    std::thread t4(&FizzBuzz::number, &fb);

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    return 0;
}
