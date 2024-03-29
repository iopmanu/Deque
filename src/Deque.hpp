#ifndef SRC_DEQUE_HPP_
#define SRC_DEQUE_HPP_

#include <algorithm>
#include <iostream>
#include <vector>
#include <cassert>

#define EXTERNAL_INIT_SIZE 2
                                                                       /*
                                                                     |  *                        *[] -> nullptr
                                                                     |  *                        *[] -> nullptr
             Data structure                                          |  * first_storage          *[] -> [uninit_zone ... value[current_first] ... values]
             organization                                            |  *                        *[] -> [       ...      values          ...            ]
                                                                     |  *                        *[] -> [       ...      values          ...            ]
The reasion to use vector as external storage instead of list is     |  * last_storage           *[] -> [values ... value[current_last] ... uninit_zone]
a random acces iterator category. Using list will give asymptotic    |  *                        *[] -> nullptr
of basic operations better, but using vector gives us the same       |  *
amortization time that we can have with using list.                  |  *   external_storage_size = 4 * initial_size - uninit_zone
                                                                     |  *   external_capacity = external_storage.size() * initial_size
                                                                     |  *
                                                                        */
template <typename T>
class Deque {
private:
    typedef T value_type;
    typedef const value_type const_value_type;

    typedef value_type* pointer;

    typedef value_type& reference;
    typedef const T& const_reference;

    const static std::size_t initial_size = 64;
    std::size_t pivot = 0;
    std::size_t current_first = (initial_size - 1) / 2 - 1;
    std::size_t current_last = (initial_size - 1) / 2;
    std::size_t first_storage = 0;
    std::size_t last_storage = 0;
    std::size_t external_storage_size = 0;
    std::size_t external_capacity = initial_size;
    std::vector<pointer> external_storage;
    
    /*This implementation use a sequence of individually allocated fixed-size arrays, with additional bookkeeping, which means indexed access to deque 
     * must perform two pointer dereferences, compared to vector's indexed access which performs only one. Expansion of a deque is cheaper than the
     * expansion of a std::vector because it does not involve copying of the existing elements to a new memory location. 

   /*===================================================================*IMPLEMENTATION*=======================================================================*/


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
        int upper_offset = static_cast<int>(this->first_storage - this->pivot);      // Данные величины вполне могут оказаться.
        int lower_offset = static_cast<int>(this->last_storage - this->pivot);       // отрицательными, особенно в случае верхнего смещения.
        this->pivot = new_pivot;
        std::size_t storage = new_pivot + upper_offset;

        for (std::size_t i = this->first_storage; i <= this->last_storage; i++, storage++) {  // Связываем новые и старые стореджи.
            new_external_storage[storage] = this->external_storage[i];
        }

        for (std::size_t i = 0; i < this->first_storage; i++) {
            delete [] this->external_storage[i];                                      // Удаляем те, в которых не было значений.
        }
        for (std::size_t i = this->last_storage + 1; i < this->external_storage.size(); i++) {
            delete [] this->external_storage[i];
        }

        this->first_storage = new_pivot + upper_offset;
        this->last_storage = new_pivot + lower_offset;
        
        for (auto& storage : new_external_storage) {                               // Выделяем кусок памяти и инициализурем сырую память значениями
            if (storage == nullptr) {                                              // конструктора по умолчанию. Собствеенно говоря по этой причине
                storage = this->make_storage();                                    // мы и удаляли эти 'незаполненные' стореджи(выше).
            }
        }

        this->external_storage = new_external_storage;
        this->external_capacity = this->external_storage.size() * this->initial_size;
    }

public:
    /*=============================================================^CONSTRUCTORS_AND_DESTRUCTORS^=============================================================*/
    
    explicit Deque() noexcept {
        this->external_storage.resize(EXTERNAL_INIT_SIZE);
        this->external_capacity = this->initial_size * 2;
        this->external_storage[0] = make_storage();
        this->external_storage[1] = make_storage();
    }

    explicit Deque(pointer source, std::size_t size) {
        for (std::size_t i = 0; i < size; i++) {
            this->push_back(source[i]);
        }
    }

    ~Deque() {
        for (auto& storage : this->external_storage) {
            delete storage;
        }
    }

    /*========================================================================^LOOKUP^========================================================================*/
    
    /*Returns the number of elements*/
    inline std::size_t get_size() const noexcept { return this->external_storage_size; }
    
    /*Returns the number of elements*/
    inline std::size_t get_capacity() const noexcept { return this->external_storage_capacity; }
    
    /*Checks whether the container is empty*/
    inline bool empty() const noexcept { return this->external_storage_size == 0; }
    
    /*Acces specified element without bounds checking*/
    reference operator[](std::size_t index) {
        index++;
        std::size_t offset = 0;

        if (this->current_first >= this->initial_size) {
            offset++;
            index -= (this->initial_size - this->current_first);
        } else {
            index += this->current_first;
        }

        offset += index / this->initial_size;
        index %= this->initial_size;

        return this->external_storage[this->first_storage + offset][index];
    }
    
    /*Access specified element with bounds checking*/
    reference at(std::size_t index) {
        assert(index <= this->external_storage_size);
        return (*this)[index];
    }
    
    /*Access the first element*/
    reference front() {
        assert(!this->empty());
        return this->operator[](0);
    }
    
    /*Acces the last element*/
    reference back() {
        assert(!this->empty());
        return this->operator[](this->external_storage_size - 1);
    }

    /*========================================================================^METHODS^=======================================================================*/
    
    /*Inserts an element to the beginning*/
    void push_front(const_reference source) {
        int current_first_int = this->current_first;

        this->external_storage[this->first_storage][this->current_first] = source;
        this->external_storage_size++;
        current_first_int--;

        if (current_first_int < 0) {
            this->current_first = this->initial_size - 1;
            int first_storage_int = this->first_storage;

            if ((first_storage_int - 1) < 0) {
                this->resize();
            }

            this->first_storage--;
        } else {
            this->current_first = static_cast<std::size_t>(current_first_int);
        }
    }
    
    /*Adds an element to the end*/
    void push_back(const_reference source) {
        int current_last_int = this->current_last;

        this->external_storage[this->last_storage][this->current_last] = source;
        this->external_storage_size++;
        current_last_int++;

        if (current_last_int >= this->initial_size) {
            this->current_last = 0;
            int last_storage_int = this->last_storage;

            if (last_storage_int + 1 >= static_cast<int>(this->external_storage.size())) {
                this->resize();
            }

            this->last_storage++;
        } else {
            this->current_last = static_cast<std::size_t>(current_last_int);
        }
    }

    void pop_back() {
        if (!this->empty()) {
            int current_last_int = this->current_last;
            current_last_int--;

            if (current_last_int < 0) {
                this->current_last = this->initial_size - 1;
                this->last_storage--;
            } else {
                this->current_last = static_cast<std::size_t>(current_last_int);
            }

            this->external_storage_size--;
        }
    }
                                                                    // По факту элемент в этих методах мы никак не удаляем,
                                                                    // да по сути эффективно удалить и не можем, поэтому наша
                                                                    // задача - просто правильно поддерживать индекс.
    void pop_front() {
        if (!this->empty()) {
            int current_first_int = this->current_first;
            current_first_int++;

            if (current_first_int >= this->initial_size) {
                this->current_first = 0;
                this->first_storage++;
            } else {
                this->current_first = static_cast<std::size_t>(current_first_int);
            }

            this->external_storage_size--;
        }
    }
    
    void print_deque() {
        for (auto& storage : this->external_storage) {
            for (std::size_t i = 0; i < this->initial_size; i++) {
                std::cout << storage[i] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "\n\n\n\n";
    }

    /*======================================================================^ITERATOR^=======================================================================*/
    
    class Iterator {
        friend class Deque;

    private:
        std::size_t current_position;
        Deque<T> *deque;

    public:
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = T*;
        using reference = T&;

        Iterator() : deque(nullptr), current_position(0) {}
        Iterator(Deque<T> *_deque, std::size_t position) : deque(_deque), current_position(position) {}

        Iterator &operator=(const Iterator &other) {
            this->current_position = other.current_position;
            this->deque = other.deque;
            return *this;
        }

        Iterator &operator+=(const std::size_t &offset) {
            *this = (*this).operator+(offset);
            return *this;
        }

        Iterator &operator-=(const std::size_t &offset) {
            *this = (*this).operator-(offset);
            return *this;
        }

        Iterator operator+(const std::size_t &offset) {
            return Iterator(this->deque, this->current_position + offset);
        }

        Iterator operator-(const std::size_t &offset) {
            return Iterator(this->deque, this->current_position - offset);
        }

        T& operator*() const {
            return (*deque)[current_position];
        }
        
        Iterator &operator++() {
            return (*this).operator+=(1);
        }

        Iterator &operator--() {
            return (*this).operator-=(1);
        }

        bool operator==(const Iterator &other) {
            return (this->deque == other.deque) && (this->current_position == other.current_position);
        }

        bool operator!=(const Iterator &other) {
            return (this->deque != other.deque) || (this->current_position != other.current_position);
        }
    };

    using iterator = Iterator;

    Iterator begin() {
        return Iterator(this, 0);
    }

    Iterator end() {
        return Iterator(this, this->get_size());
    }

    /*======================================================================^STREAM^=======================================================================*/

    friend std::ostream &operator<<(std::ostream& out, Deque<T>* source) noexcept {
        if (source == NULL) {
            return out;
        }

        for (auto it = source->begin(); it != source->end(); it.operator++()) {
            out << it.operator*() << " ";
        }
        out << std::endl;

        return out;
    }

    void insert(iterator it, const T& source) {
        if (it == this->end()) {
            this->push_back(source);
        } else {
            T copy = it.operator*();
            it.operator*() = source;

            for (auto deque_iterator = it.operator++(); deque_iterator != this->end(); deque_iterator.operator++()) {
                T buffer = deque_iterator.operator*();
                deque_iterator.operator*() = copy;
                copy = buffer;
            }

            this->push_back(copy);
        }
    }

    void erase(iterator it) {
        for (auto deque_iterator = it; deque_iterator != this->end(); deque_iterator.operator++()) {
            *(deque_iterator - 1) = *(deque_iterator);
        }

        this->pop_back();
    }
};

#endif // SRC_DEQUE_HPP_
