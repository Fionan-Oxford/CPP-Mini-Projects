#include <iostream>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>

template<typename T>
class BoundedBlockingQueue {
public:
    BoundedBlockingQueue(size_t capacity) : capacity_(capacity) {}

    void enqueue(const T& item) {
        std::unique_lock<std::mutex> lock(mutex_);
        not_full_.wait(lock, [this]() { return queue_.size() < capacity_; });

        queue_.push(item);

        // Notify one waiting consumer
        not_empty_.notify_one();
    }

    T dequeue() {
        std::unique_lock<std::mutex> lock(mutex_);
        not_empty_.wait(lock, [this]() { return !queue_.empty(); });

        T item = queue_.front();
        queue_.pop();

        // Notify one waiting producer
        not_full_.notify_one();
        return item;
    }

    size_t size() {
        std::lock_guard<std::mutex> lock(mutex_);
        return queue_.size();
    }

private:
    std::queue<T> queue_;
    size_t capacity_;
    std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

// === Demonstration ===

void producer(BoundedBlockingQueue<int>& queue, int id, int num_items) {
    for (int i = 0; i < num_items; ++i) {
        int item = id * 100 + i; // Unique item
        queue.enqueue(item);
        std::cout << "Producer " << id << " enqueued " << item << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
}

void consumer(BoundedBlockingQueue<int>& queue, int id, int num_items) {
    for (int i = 0; i < num_items; ++i) {
        int item = queue.dequeue();
        std::cout << "Consumer " << id << " dequeued " << item << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }
}

int main() {
    const int queue_capacity = 5;
    const int num_items_per_thread = 5;

    BoundedBlockingQueue<int> queue(queue_capacity);

    std::thread p1(producer, std::ref(queue), 1, num_items_per_thread);
    std::thread p2(producer, std::ref(queue), 2, num_items_per_thread);
    std::thread c1(consumer, std::ref(queue), 1, num_items_per_thread);
    std::thread c2(consumer, std::ref(queue), 2, num_items_per_thread);

    p1.join();
    p2.join();
    c1.join();
    c2.join();

    std::cout << "Final queue size: " << queue.size() << "\n";

    return 0;
}
