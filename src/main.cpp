#include <iostream>
#include "Deque.hpp"

// 60 50 15 10 5 3 1 | 6 7 2 4 20 21 100

int main() {
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
    deque->print_deque();

    std::cout << deque->front() << " " << deque->back() << std::endl;
    std::cout << deque->at(3) << " " << deque->operator[](2);
    deque->pop_back();
    //std::cout << std::endl << deque->back();
    deque->pop_front();
    //std::cout << std::endl << deque->front();
    return 0;
}
