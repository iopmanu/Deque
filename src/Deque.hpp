#ifndef SRC_DEQUE_HPP_
#define SRC_DEQUE_HPP_

#include <iostream>
#include <algorithm>
#include <vector>
                                                                                           /*
                                                                     |                      *                        *[] -> nullptr
                                                                     |                      *                        *[] -> nullptr
             Data structure                                          |                      * first_storage          *[] -> [uninit_zone ... value[current_first] ... values]
             organization                                            |                      *                        *[] -> [       ...      values          ...            ]
                                                                     |                      *                        *[] -> [       ...      values          ...            ]
The reasion to use vector as external storage instead of list is     |                      * last_storage           *[] -> [values ... value[current_last] ... uninit_zone]
a random acces iterator category. Using list will give asymptotic    |                      *                        *[] -> nullptr
of basic operations better, but using vector gives us the same       |                      *
amortization time that we can have with using list.                  |                      *   external_storage_size = 4
                                                                     |                      *   external_capacity = external_storage.size() * initial_size
                                                                     |                      *                        
                                                                                            */
template<typename T>
class Own_deque {  
private:
    typedef T value_type;
    typedef value_type* pointer;
    typedef value_type& reference;

    const std::size_t initial_size = 32;
    std::size_t pivot = 0;
    std::size_t current_first = (initial_size - 1) / 2;
    std::size_t current_last = (initial_size - 1) / 2 - 1;
    std::size_t first_storage = 0;
    std::size_t last_storage = 0;
    std::size_t external_storage_size = 0;
    std::size_t external_capacity = initial_size;
    std::vector<pointer> external_storage;
    
    
    /* Вообще оператор new может не вызывать конструктор по умолчанию и выдать просто кусок сырой памяти, что вызовет ub */
    pointer make_storage() noexcept {
        pointer new_storage = reinterpret_cast<T*>(new char[this->initial_size * sizeof(value_type)]);
        return new_storage;
    }
    
    /* Дисклеймер (ЗДЕСЬ МОГ БЫТЬ ВАШ ЛИСТ), однако определенным образом подбирая initial_size - константа степени 2(символично),
     * мы можем практически избежать вызова метода resize, поддерживать операции за все те же O(1). Метод крайне простоват:
     * создаем новый внешний сторедж(вектор) размер X2, затем присваиваем внутренним стореджам внутренние стореджи старого вектора.
     * При этом по сути поэлементного копирования не происходит. (Вся сложность поддерживать индексы как на примере в самом верху) */
    void resize() noexcept {
        std::size_t new_size = this->external_storage.size() * 2;
        std::vector<pointer> new_external_storage(new_size);
        std::size_t new_pivot = new_size / 2 - 1;
        int upper_offset = static_cast<int>(this->first_storage - new_pivot); // Данные величины вполне могут оказаться.
        int lower_offset = static_cast<int>(this->last_storage - new_pivot);  // отрицательными, особенно в случае верхнего смещения.
        this->pivot = new_pivot;
        std::size_t upper_storage = new_pivot + upper_offset;

        for (std::size_t i = this->first_storage; i <= this->last_storage; i++) { // Связываем новые и старые стореджи.
            new_external_storage[i] = this->external_storage[i];
        }

        for (std::size_t i = 0; i < this->first_storage; i++) {
            delete this->external_storage[i];                                                           // Удаляем те, в которых не было значений.
        }
        for (std::size_t i = this->last_storage + 1; i < this->external_storage.size(); i++) {
            delete this->extenral_storage[i];
        }

        this->first_storage = pivot + upper_offset;
        this->last_storage = pivot + lower_offset;

        for (auto &storage : new_external_storage) {          // Выделяем кусок памяти и инициализурем сырую память значениями
            if (storage == nullptr) {                         // конструктора по умолчанию. Собствеенно говоря по этой причине
                storage = this->make_storage();               // мы и удаляли эти 'незаполненные' стореджи(выше).
            }
        }

        this->external_storage = new_external_storage;
        this->external_capacity = this->external_storage.size() * this->initial_size;
    }
};

#endif
