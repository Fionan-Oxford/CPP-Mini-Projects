#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
#include <stdexcept>


class LRUCache{
public:
    LRUCache(size_t capacity): cap(capacity), size_(0) {
        if(cap == 0){
            throw std::runtime_error("Capacity cannot be 0!");
        }
    }

    void push(int key, int value){
        if(lru_map.find(key) != lru_map.end()){ //already exists
            lru_list.emplace_front(key, value);
            auto front = lru_list.begin();
            lru_map[key] = front;
            return;
        }
        //Is new
        if(cap >= size_){
            auto least = lru_list.back();
            lru_list.pop_back();
            lru_map.erase(least.first);
            size_--;
        }
        lru_list.emplace_front(key, value);
            auto front = lru_list.begin();
            lru_map[key] = front;
            size_++;
    }



private:
    using lru_type = std::list<std::pair<int, int>>;
    lru_type lru_list;
    std::unordered_map<int, lru_type::iterator> lru_map;
    size_t size_;
    size_t cap;
};