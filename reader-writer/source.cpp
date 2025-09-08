#include <iostream>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <thread>


class RWLock{
    public:
        RWLock():active_readers_(0), waiting_writers_(0), writer_active_(false){}

        void readLock(){
            std::unique_lock<std::mutex> lock(mtx_);
            cond_.wait(lock, [this](){
                return !writer_active_ && waiting_writers_ == 0;    
            });
            ++active_readers_;
        }

        void readUnlock(){
            std::lock_guard<std::mutex> lock(mtx_);
            --active_readers_;
            if (active_readers_ == 0) cond_.notify_all();
        }

        void writeLock(){
            std::unique_lock<std::mutex> lock(mtx_);
            ++waiting_writers_;
            cond_.wait(lock,[this](){
                return !writer_active_ && active_readers_ == 0;
            });
            --waiting_writers_;
            writer_active_ = true;
        }

        void writeUnlock(){
            std::lock_guard<std::mutex> lock(mtx_);
            writer_active_ = false;
            cond_.notify_all();
        }

    

    private:
        std::mutex mtx_;
        std::condition_variable cond_;
        int active_readers_;
        int waiting_writers_;
        bool writer_active_;

};

RWLock lock;

void reader(int id){
    lock.readLock();
    std::cout << "Reader "<< id<< " is reading" <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Reader " << id << " is done" << std::endl;
    lock.readUnlock();
}

void writer(int id){
    lock.writeLock();
    std::cout << "Writer "<< id<< " is writing" <<std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    std::cout << "Writer " << id << " is done" << std::endl;
    lock.writeUnlock();
}

int main(){
    std::vector<std::thread> threads;

    //Start readers and writers in a mixed order
    threads.emplace_back(reader, 1);
    threads.emplace_back(reader, 2);
    threads.emplace_back(writer, 1);
    threads.emplace_back(reader, 3);
    threads.emplace_back(writer, 2);
    threads.emplace_back(reader, 4);

    for(auto &t : threads) t.join();

    return 0;

}

