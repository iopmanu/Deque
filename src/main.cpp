#include <iostream>
#include <deque>
#include <chrono>
#include <vector>
#include <stdlib.h>

#include "Deque.hpp"

using namespace std::chrono;

void testing_with_stl_push_back();
void testing_with_stl_push_front();
// 60 50 15 10 5 3 1 | 6 7 2 4 20 21 100

int main() {
    
    testing_with_stl_push_back();
    testing_with_stl_push_front();
    auto deque = new Deque<int>();
    deque->push_front(1);
    deque->push_front(3);
    deque->push_back(6);
    deque->push_front(5);
    deque->push_back(7);
    deque->push_front(10);
    deque->push_front(15);
    deque->push_back(2);
    deque->push_back(4);
    deque->push_back(20);
    deque->push_back(21);
    deque->push_back(100);
    deque->push_front(50);
    deque->push_front(60);

    //std::cout << deque->front() << " " << deque->back() << std::endl;
    //std::cout << deque->at(3) << " " << deque->operator[](2);
    //deque->pop_back();
    auto it = deque->begin().operator+=(5);

    std::cout << *it << std::endl;
    deque->insert(it, 1234);
    deque->erase(it);

    std::cout << deque;
    /*
    //std::cout << std::endl << deque->back();
    deque->pop_front();
    //std::cout << std::endl << deque->front();
    return 0;
    */
}


void testing_with_stl_push_back() {
    srand(time(NULL));
    std::cout << "Testing push_back \n\n";
    auto deque = new Deque<int>;
    auto stl_deque = new std::deque<int>;

    std::vector<int> v;

    for (std::size_t i = 0; i < 10000; i++) {
        v.push_back(1 + rand() % 100);
    }
    
    auto start_clock_my = high_resolution_clock::now();
    for (std::size_t i = 0; i < 10000; i++) {
        deque->push_back(v[i]);
    }
    auto stop_clock_my = high_resolution_clock::now();
    auto duration_my = stop_clock_my - start_clock_my;

    auto start_clock_stl = high_resolution_clock::now();
    for (std::size_t i = 0; i < 10000; i ++) {
        deque->push_back(v[i]);
    }
    auto stop_clock_stl = high_resolution_clock::now();
    auto duration_stl = stop_clock_stl - start_clock_stl;
    
    std::cout << "\t\t My \t STL"  << std::endl;
    std::cout << "Duration:\t" << duration_my.count() << "\t" << duration_stl.count() << "\n\n\n";
}

 void testing_with_stl_push_front() {
     srand(time(NULL));
     std::cout << "Testing push_front \n\n";
     auto deque = new Deque<int>;
     auto stl_deque = new std::deque<int>;
 
     std::vector<int> v;
 
     for (std::size_t i = 0; i < 10000; i++) {
         v.push_back(1 + rand() % 100);
     }
     
     auto start_clock_my = high_resolution_clock::now();
     for (std::size_t i = 0; i < 10000; i++) {
         deque->push_front(v[i]);
     }
     auto stop_clock_my = high_resolution_clock::now();
     auto duration_my = stop_clock_my - start_clock_my;
 
     auto start_clock_stl = high_resolution_clock::now();
     for (std::size_t i = 0; i < 10000; i ++) {
         deque->push_front(v[i]);
     }
     auto stop_clock_stl = high_resolution_clock::now();
     auto duration_stl = stop_clock_stl - start_clock_stl;
 
     std::cout << "\t\t My \t STL"  << std::endl;
     std::cout << "Duration:\t" << duration_my.count() << "\t" << duration_stl.count() << "\n\n\n";
 }

