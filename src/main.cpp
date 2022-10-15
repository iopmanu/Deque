#include <iostream>
#include "Deque.hpp"

// 15 10 5 3 1 6 7 2 4

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
    std::cout << deque->front() << " " << deque->back() << std::endl;
    std::cout << deque->at(2) << " " << deque->operator[](2);
    return 0;
}
