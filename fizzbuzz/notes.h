#include <iostream>
#include <future>
#include <vector>

// FUnction to compute the sum of squares in a range
int sum_of_squares(int start, int end){
    int sum = 0;
    for(int i = start; i<= end; ++i){
        sum += i * i;
    }
    return sum;
}

int main(){
    int N = 100;
    int num_segments = 4;

    std::vector<std::future<int>> futures;
    int segment_size = N / num_segments;

    // Launch async tasks
    for (int i = 0; i < num_segments; ++i){
        int start = i * segment_size + 1;
        int end = (i+1)*segment_size;
        futures.push_back(std::async(std::launch::async, sum_of_squares, start, end));
    }

    //combine results
    int total_sum = 0;
    for(auto & fut : futures){
        total_sum += fut.get();
    }

    std::cout << "Sum is " << total_sum << std::endl;

    return 0;
}