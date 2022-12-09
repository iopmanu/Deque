#include <chrono>
#include <deque>
#include <iostream>
#include <stdlib.h>
#include <vector>

#include "Deque.hpp"

using namespace std::chrono;

void testing_with_stl_push_back();
void testing_with_stl_push_front();
void testing_push_pop();
// 60 50 15 10 5 3 1 | 6 7 2 4 20 21 100

int main() {
    testing_with_stl_push_back();
    testing_with_stl_push_front();
    testing_push_pop();
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
    for (std::size_t i = 0; i < 10000; i++) {
        deque->push_back(v[i]);
    }
    auto stop_clock_stl = high_resolution_clock::now();
    auto duration_stl = stop_clock_stl - start_clock_stl;

    std::cout << "\t\t My \t STL" << std::endl;
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
    for (std::size_t i = 0; i < 10000; i++) {
        deque->push_front(v[i]);
    }
    auto stop_clock_stl = high_resolution_clock::now();
    auto duration_stl = stop_clock_stl - start_clock_stl;

    std::cout << "\t\t My \t STL" << std::endl;
    std::cout << "Duration:\t" << duration_my.count() << "\t" << duration_stl.count() << "\n\n\n";
}

void testing_push_pop() {
    std::vector v = {60, 50, 15, 10, 5, 3, 1, 6, 7, 2, 4, 20, 21, 100};
    auto deque = new Deque<int>;
    auto stl_deque = new std::deque<int>;

    for (int i = 0; i < v.size(); i++) {
        deque->push_back(v[i]);
        stl_deque->push_back(v[i]);
    }

    auto my_it = deque->begin();

    for (auto it = stl_deque->begin(); it != stl_deque->end() && my_it != deque->end(); it++, my_it.operator++()) {
        assert(*it == *my_it);
    }

    for (int i = v.size() - 1; i >= 0; i--) {
        deque->push_front(v[i]);
        stl_deque->push_front(v[i]);
    }

    my_it = deque->begin();
    for (auto it = stl_deque->begin(); it != stl_deque->end() && my_it != deque->end(); it++, my_it.operator++()) {
        assert(*it == *my_it);
    }

    for (int i = 0; i < 3; i++) {
        deque->pop_front();
        stl_deque->pop_front();
    }

    my_it = deque->begin();
    for (auto it = stl_deque->begin(); it != stl_deque->end() && my_it != deque->end(); it++, my_it.operator++()) {
        assert(*it == *my_it);
    }

    for (int i = 0; i < 5; i++) {
        deque->pop_back();
        stl_deque->pop_back();
    }

    my_it = deque->begin();
    for (auto it = stl_deque->begin(); it != stl_deque->end() && my_it != deque->end(); it++, my_it.operator++()) {
        assert(*it == *my_it);
    }

    assert(deque->get_size() == stl_deque->size());

    my_it = deque->begin().operator+=(5);
    auto it = stl_deque->begin() + 5;

    assert(*my_it == *it);

    my_it = my_it.operator++();
    it = it.operator++();
    assert(*my_it == *it);

    my_it = my_it.operator-(2);
    it = it - 2;
    assert(*my_it == *it);

    my_it.operator-=(2);
    it -= 2;
    assert(*my_it == *it);

    for (int i = 1; i < 2; i++, my_it.operator++(), it++) {
        deque->insert(my_it, i * 123);
        stl_deque->insert(it, i * 123);
    }

    my_it = deque->begin();
    for (auto it = stl_deque->begin(); it != stl_deque->end() && my_it != deque->end(); it++, my_it.operator++()) {
        assert(*it == *my_it);
    }

}
