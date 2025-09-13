/*
Problem: Minimal Ring Buffer (ints)

Task
Implement a fixed-capacity circular queue of int that never reallocates.

API (exact names)

class RingBuffer {
explicit RingBuffer(size_t capacity); // capacity > 0 or throw std::invalid_argument
bool push(int x); // insert at tail; return false if full
bool pop(int& out); // remove from head into out; false if empty
int& front(); const int& front() const; // pre: not empty
int& back(); const int& back() const; // pre: not empty
size_t size() const noexcept;
size_t capacity() const noexcept;
bool empty() const noexcept;
bool full() const noexcept;
void clear() noexcept; // drop all elements, keep capacity
};

Constraints

Preallocate storage once (e.g., std::vector<int> buf; buf.resize(capacity);).
Maintain indices with modulo arithmetic.
All operations O(1) time; O(C) space.
State model (keep it simple)
Fields: std::vector<int> buf; size_t cap; size_t head=0; size_t sz=0;
Logical element i (0 ≤ i < sz) lives at buf[(head + i) % cap].
Tail index is (head + sz) % cap.

Algorithms
push(x): if sz == cap → false; else write at tail, sz++, true.
pop(out): if sz == 0 → false; else read buf[head] to out, head = (head + 1) % cap, sz--, true.
front() = buf[head]; back() = buf[(head + sz - 1) % cap].

Edge cases to handle
Capacity 1 (wrap correctly).
Repeated wrap-around (push/pop many times).
clear() resets head = 0; sz = 0; (don’t resize).

*/
#include <vector>
#include <stdexcept>
#include <atomic>
#include <memory>
#include <cassert>

class RingBuffer {
private:
    size_t cap;
    std::vector<int> buffer;
    std::atomic<int> head;
    std::atomic<int> tail;
    std::atomic<size_t> size_;

    // 0 1 2
    // x      tail = 1 head = 0
    // x x    tail = 2, head = 0

public:
    explicit RingBuffer(size_t capacity): cap(capacity), buffer(cap), head(0), tail(0), size_(0){
        if(capacity ==0){
            throw std::invalid_argument("Capacity must be greater than 0");
        }
    } // capacity > 0 or throw std::invalid_argument

    bool push(const int& x){

        auto current_tail = tail.load(std::memory_order_acquire);

        auto next_tail = (current_tail + 1) % cap;

        if(full()){
            return false; //We are full
        }

        buffer[current_tail] = x;

        tail.store(next_tail, std::memory_order_release);
        size_ ++;
        return true;
    } // insert at tail; return false if full

    bool pop(int& out){
        auto current_head = head.load(std::memory_order_acquire);
        auto current_tail = tail.load(std::memory_order_relaxed);

        if(empty()){
            return false; //empty
        }

        out = buffer[current_head];
        auto next_head = ((current_head + 1)) % cap;

        head.store(next_head, std::memory_order_release);
        size_--;
        return true;

    } // remove from head into out; false if empty

    int& front(){
        if(empty()){
            throw std::runtime_error("IS empty");
        }
        auto current_head = head.load(std::memory_order_relaxed);
        return buffer[current_head];
    } // pre: not empty

    int& back(){
        if(empty()){
            throw std::runtime_error("IS empty");
        }
        auto current_tail = tail.load(std::memory_order_relaxed);
        return buffer[current_tail];

    }// pre: not empty
    size_t size() const noexcept{
        return size_;
    }
    size_t capacity() const noexcept{
        return cap;
    }
    bool empty() const noexcept{
        if(size_ == 0){
            return true; //empty
        }else{
            return false;
        }
    }
    bool full() const noexcept{
        if(size_ == cap){
            return true; //We are full
        }
        return false;
    }

    void clear() noexcept{
        auto current_head = head.load(std::memory_order_acquire);
        auto current_tail = tail.load(std::memory_order_relaxed);
        tail.store(current_head, std::memory_order_release);
    } // drop all elements, keep capacity
};

int main(){

    RingBuffer q(3);
    assert(q.empty());
    assert(q.push(1));
    assert(q.push(2));
    assert(q.push(3));
    assert(!q.push(4));          // full
    int x;
    assert(q.pop(x) && x==1);    // now [2,3]
    assert(q.push(4));           // [2,3,4]
    assert(q.front()==2);
    assert(q.back()==4);
    assert(q.size()==3 && q.full());
    q.clear();
    assert(q.empty() && !q.full());

    return 0;
}