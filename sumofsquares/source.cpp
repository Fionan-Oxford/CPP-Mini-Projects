#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>


class FizzBuzz{
    public:
        FizzBuzz(int n): n_(n){}

        void fizz(){
            while(counter <=n_){
            std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){return counter % 3 == 0 && counter %5 != 0;});
                std::cout << "fizz" << std::endl;
                counter++;
                cv.notify_all();
            }
        }
        void buzz(){
            while(counter <=n_){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){return counter % 3 != 0 && counter %5 == 0;});
                std::cout << "buzz" << std::endl;
                counter++;
                cv.notify_all();
            }
        }
        void fizzbuzz(){
            while(counter <=n_){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){return counter % 3 == 0 && counter %5 == 0;});
                std::cout << "fizzbuzz" << std::endl;
                counter++;
                cv.notify_all();
            }
        }
        void number(){
            while(counter <=n_){
                std::unique_lock<std::mutex> lock(mtx);
                cv.wait(lock, [this](){return counter % 3 != 0 && counter %5 != 0;});
                std::cout << counter<< std::endl;
                counter++;
                cv.notify_all();
            }
        }
    private:
        int n_;
        std::mutex mtx;
        int counter{1};
        std::condition_variable cv;

};

int main(){

    FizzBuzz fb(30);
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