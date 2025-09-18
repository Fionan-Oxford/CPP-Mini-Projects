#include <iostream>
#include <thread>
#include <atomic>
#include <future>
#include <memory>
#include <mutex>
#include <queue>
#include <vector>
#include <functional>
#include <condition_variable>

class ThreadPool {
public:
    ThreadPool(int numThreads) : stop(false) {
        for (int i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(queueMutex);
                        cv.wait(lock, [this]() { return stop || !tasks.empty(); });

                        if (stop && tasks.empty()) {
                            return;
                        }

                        task = std::move(tasks.front());
                        tasks.pop(); // Move the task and pop it
                    }

                    task();
                }
            });
        }
    }

    ~ThreadPool() {
        stop = true;
        cv.notify_all();

        for (auto& worker : workers) {
            worker.join();
        }
    }

    // Enqueue
    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) {
        using return_type = std::invoke_result_t<F, Args...>;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );

        std::future<return_type> result = task->get_future();

        {
            std::unique_lock<std::mutex> lock(queueMutex);

            if (stop) {
                throw std::runtime_error("Can't enqueue on a stopped thread");  // ✅ FIXED
            }

            tasks.emplace([task]() { (*task)(); });
        }

        cv.notify_one();
        return result;
    }

private:
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
    std::mutex queueMutex;
    std::condition_variable cv;
    std::atomic<bool> stop;
};

int main() {
    ThreadPool pool(4); // Create a pool with 4 threads

    auto future1 = pool.enqueue([] {
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        return std::string("Hello from the thread pool!");
    });

    auto future2 = pool.enqueue([](int a, int b) {
        return a * b;
    }, 6, 7);

    std::cout << "Waiting for results...\n";
    std::cout << "Result 1: " << future1.get() << "\n";
    std::cout << "Result 2: " << future2.get() << "\n";

    return 0;
}
