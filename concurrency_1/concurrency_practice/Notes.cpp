#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <mutex>

std::list<int> g_Data;
const int SIZE{10000};
std::mutex g_Mutex;

void Download(){
    for(int i  = 0; i < SIZE; ++i ){
        g_Mutex.lock();
        g_Data.push_back(i);
        g_Mutex.unlock();
    }
}

void Download2(){
    for(int i  = 0; i < SIZE; ++i ){
        g_Mutex.lock();
        g_Data.push_back(i);
        g_Mutex.unlock();
    }
}

int main(){


    std::thread thDownloader(Download);
    std::thread thDownloader2(Download2);

    thDownloader.join();
    thDownloader2.join();

    std::cout << g_Data.size() << std::endl;
    return 0;
}

/////////////////////
#include <future>
#include <iostream>
#include <thread>
#include <vector>

int Add(int x, int y){
    return x+ y;
}

int Square(int x){
    return x * x;
}

int main(){

    //How do we get the return value?

    //We use the package class
    std::packaged_task<int(int,int)> taskAdd{Add};
    std::future<int> ft =taskAdd.get_future();

    std::thread t1(std::move(taskAdd),3,5);

    
    t1.join();
    std::cout << ft.get() << std::endl;
}

//////////
#include <iostream>
#include <list>
#include <thread>
#include <string>
#include <mutex>

std::list<int> g_Data;
const int SIZE{10000};
std::mutex g_Mutex;

void Download(){
    for(int i  = 0; i < SIZE; ++i ){
        std::lock_guard<std::mutex> mtx(g_Mutex);
        g_Data.push_back(i);
    }
}

void Download2(){
    for(int i  = 0; i < SIZE; ++i ){
        std::lock_guard<std::mutex> mtx(g_Mutex);
        g_Data.push_back(i);
    }
}

int main(){


    std::thread thDownloader(Download);
    std::thread thDownloader2(Download2);

    thDownloader.join();
    thDownloader2.join();

    std::cout << g_Data.size() << std::endl;
    return 0;
}

#include <iostream>
#include <thread>
#include <chrono>
#include <future>

int Download(int count){
    using namespace std::chrono_literals;
    int sum{};
    for (int i=0; i<10; i++){
        sum += i;
        std::cout << '.';
        std::this_thread::sleep_for(300ms) ;
    }
    return sum;
}

int main(){
    using namespace std::chrono_literals;
    std::future<int> result = std::async(std::launch::deferred, Download, 10);
    std::this_thread::sleep_for(1s);
    std::cout << "Finished Main" << std::endl;
    if (result.valid()){
        auto sum = result.get();
        std::cout << sum << std::endl;
    }

    return 0;
}