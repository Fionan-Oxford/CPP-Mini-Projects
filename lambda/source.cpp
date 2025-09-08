#include <iostream>

int main(){

    auto fn = [](){
        std::cout << "Lambda "<< std::endl;
    };

    fn();

    auto sum = [](int x, int y){return x + y};

    return 0;
}