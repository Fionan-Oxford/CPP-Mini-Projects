#include <iostream>
#include <future>
#include <vector>


int sum_of_squares(int start, int end){
    int total_sum = 0;

    for(int i = start; i < end; ++i){
        total_sum += i*i;
    }
    return total_sum;
}

int main(){
    int counter = 100;
    int NUM_THREADS = 4;
    std::vector<std::future<int>> futures;

    int size_squares = counter/NUM_THREADS;

    for(int i = 0; i < NUM_THREADS; ++i){
        int start = i*size_squares + 1;
        int end = (i == NUM_THREADS - 1) ? (counter + 1) : (i + 1) * size_squares + 1;
        futures.push_back(std::async(std::launch::async, sum_of_squares, start, end));
    }

    int total_sum{0};
    for(auto &fut : futures){
        total_sum += fut.get();
    }

    std::cout << total_sum << std::endl;

    return 0;

}