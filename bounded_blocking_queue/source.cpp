/*Implement a thread-safe,
bounded-capacity FIFO queue for generic T
that supports multiple producers and multiple consumers.
Operations block when the queue is full/empty and wake appropriately.


Thread safety: Safe for multiple producers and multiple consumers.
Blocking semantics: Use std::mutex + std::condition_variable. No busy-waiting.
Spurious wakeups: Handle via while predicates (not if).
Ordering: FIFO by enqueue order.
Complexity: O(1) per op (amortized), O(capacity) space.
Storage: Preallocate a ring buffer of size capacity; never reallocate.
Move semantics: Prefer moving T where possible; support move-only types.
Exception safety:
If element move/copy throws during push, the queue state remains unchanged; any waiting threads remain correctly synchronized (strong guarantee).
No exceptions escape destructors.
Close semantics: close() is idempotent, unblocks all waiters, and does not destroy queued items.
Notifications: Use notify_one when waking a single complementary waiter; notify_all on close().*/

#include <atomic>
#include <chrono>
#include <vector>
#include <stdexcept>
#include <condition_variable>
#include <thread>
#include <mutex>
#include <iostream>


template <class T>
class BoundedBlockingQueue{
public:
    explicit BoundedBlockingQueue(int capacity): cap(capacity), buffer(capacity), head(0), tail(0), size_(0), closed(false){
        if(cap <= 0){
            throw std::invalid_argument("Capacity must be > 0 ");
        }
    }

    void push(T item){
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv.wait(lock, [this](){return closed || size_ < cap;});
            if(closed) throw std::runtime_error("Cannot push to a closed queue");
            buffer[tail] = std::move(item);
            int next_tail = (tail + 1) % cap;
            tail = next_tail;
            ++size_;
        }

        cv.notify_one();
    }


    void pop(T& out){
        {
            std::unique_lock<std::mutex> lock(mtx_);
            cv.wait(lock,[this](){return closed || size_ > 0;});
            if(closed && size_ == 0) throw std::runtime_error("Cannot pop from a closed queue");

            out = std::move(buffer[head]);
            int next_head = (head + 1) % cap;
            head = next_head;
            --size_;
        }

        cv.notify_one();
    }


    bool try_push_for(T item, std::chrono::milliseconds timeout);

    bool try_pop_for(T& out, std::chrono::milliseconds timeout);

    void close() noexcept;

private:
    std::vector<T> buffer;
    int head;
    int tail;
    int size_;
    int cap;
    bool closed;
    std::mutex mtx_;
    std::condition_variable cv;

};

int main(){
    BoundedBlockingQueue<int> bbq(3);
    bbq.push(1);
    bbq.push(2);
    bbq.push(3);
    int out;
    bbq.pop(out);
    std::cout << out << std::endl;
    bbq.pop(out);
    std::cout << out << std::endl;
    bbq.pop(out);
    std::cout << out << std::endl;
    return 0;
}