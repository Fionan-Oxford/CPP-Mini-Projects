#include <unordered_map>
#include <list>
#include <iostream>

/*
For o(1) acess, I need to use an unordered map
Key -> generates an iterator to the list
the list is a s

I need a list, with an iterator used to acess
My data will take the form of a key and value. 



*/

class LRUCache{
    private:
        int capacity;

        //List stores keys and values
        std::list<std::pair<int, int>> cacheList;

        //Map uses key to iterator
        std::unordered_map<int, std::list<std::pair<int, int>>::iterator> cacheMap;

    public:
        LRUCache(int capacity): capacity(capacity){}

        int get(int key){
            //Key not found
            if(cacheMap.find(key) == cacheMap.end()){
                return -1;
            }

            //Move the accessed one to the front. 
            auto it = cacheMap[key];
            int value = it->second;

            cacheList.erase(it);
            cacheList.push_front({key, value});
            cacheMap[key] = cacheList.begin();

            return value;
        }

        void put(int key, int value){
            if(cacheMap.find(key) != cacheMap.end()){
                auto it = cacheMap[key];
                cacheList.erase(it);
                cacheList.push_front({key,value});
                cacheMap[key] = cacheList.begin();
                return;
            }

            //Doesn't already exist.
            if(cacheList.size() == capacity){
                auto lru = cacheList.back();
                int lruKey = lru.first;

                cacheList.pop_back();
                cacheMap.erase(lruKey);
            }

            //Insert new key value pair. 
            cacheList.push_front({key,value});
            cacheMap[key] = cacheList.begin();
        }
};