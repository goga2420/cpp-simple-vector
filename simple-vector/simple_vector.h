#pragma once
#include <iostream>
#include <numeric>
#include <cassert>
#include <initializer_list>
#include <stdexcept>
#include<new>
#include <utility>
using namespace std;
struct ReserveProxyObj {
    ReserveProxyObj(size_t n) {
        capacity_ = n;
    }
    size_t GetCapacity() {
        return capacity_;
    }
private:
    size_t capacity_;
};

template <typename Type>
class SimpleVector {
public:
    using Iterator = Type*;
    using ConstIterator = const Type*;
    SimpleVector() noexcept {
        size_ = 0;
        capacity_ = 0;
        items_ = std::move(nullptr);
    }
    // Создаёт вектор из size элементов, инициализированных значением по умолчанию
    explicit SimpleVector(size_t size) {
        // Напишите тело конструктора самостоятельно
        size_ = size;
        capacity_ = size;
        if (size_ > 0) {
            items_ = new Type[size_];
            std::fill(&items_[0], &items_[size_], Type());
        }
    }
    SimpleVector(ReserveProxyObj reserve) {
        capacity_ = 0;
        size_ = 0;
        items_ = nullptr;
        this->Reserve(reserve.GetCapacity());
    }
    // Создаёт вектор из size элементов, инициализированных значением value
    SimpleVector(size_t size, const Type& value) {
        size_ = size;
        capacity_ = size;
        if (size_ > 0) {
            items_ = new Type[size_];
            std::fill(&items_[0], &items_[size_], value);
        }
        // Напишите тело конструктора самостоятельно
    }


    // Создаёт вектор из std::initializer_list
    SimpleVector(std::initializer_list<Type> init) {
        if (init.size() > 0) {
            items_ = new Type[init.size()];
            size_ = init.size();
            capacity_ = init.size();
            std::fill(&items_[0], &items_[size_], Type());
            std::copy(init.begin(), init.end(), &items_[0]);
        }
        // Напишите тело конструктора самостоятельно
    }
    ~SimpleVector() {
        delete[] items_;
    }

    SimpleVector(const SimpleVector& other) {
        // Напишите тело конструктора самостоятельно
        *this = other;
    }
    
    SimpleVector(SimpleVector&& other) {
        // Напишите тело конструктора самостоятельно
        *this = std::move(other);
    }
    
    void Reserve(size_t new_capacity) {
        if (new_capacity > capacity_) {
            Type* temp_items = new Type[new_capacity];
            std::copy(&items_[0], &items_[size_], &temp_items[0]);
            capacity_ = new_capacity;
            delete[] items_;
            items_ = std::move(temp_items);
        }
    }

    SimpleVector& operator=(const SimpleVector& rhs) {

        // Напишите тело конструктора самостоятельно
        Type* temp_items = new Type[rhs.capacity_];
        std::copy(rhs.begin(), rhs.end(), &temp_items[0]);
        if (items_ == rhs.items_)
            return *this;
        std::swap(items_, temp_items);
        size_ = rhs.size_;
        capacity_ = rhs.capacity_;
        return *this;
    }

    SimpleVector& operator= (SimpleVector&& rhs) {
        if (this == &rhs)
            return *this;
        Type* temp_items = new Type[rhs.capacity_];
        int counter = 0;
        for (auto i = rhs.begin(); i < rhs.end(); ++i) {
            temp_items[counter] = std::move(*i);
            counter++;
        }
        items_ = std::move(temp_items);
        size_ = std::move(rhs.size_);
        capacity_ = std::move(rhs.capacity_);
        rhs.size_ = 0;
        rhs.capacity_ = 0;
        return *this;
    }

    // Добавляет элемент в конец вектора
    // При нехватке места увеличивает вдвое вместимость вектора
    void PushBack(const Type& item) {
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            Type* temp_items = new Type[new_capacity];
            std::copy(&items_[0], &items_[size_], &temp_items[0]);
            capacity_ = new_capacity;
            delete[] items_;
            items_ = temp_items;
        }
        items_[size_] = item;
        ++size_;
        
    }

    void PushBack(Type&& item) {
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            Type* temp_items = new Type[new_capacity];
            int counter = 0;
            for (size_t i = 0; i < size_; ++i)
            {
                temp_items[counter] = std::move(items_[i]);
                ++counter;
            }
            capacity_ = new_capacity;
            delete[] items_;
            items_ = std::move(temp_items);
        }
        items_[size_]=std::move(item);
        ++size_;
        
    }

    // Вставляет значение value в позицию pos.
    // Возвращает итератор на вставленное значение
    // Если перед вставкой значения вектор был заполнен полностью,
    // вместимость вектора должна увеличиться вдвое, а для вектора вместимостью 0 стать равной 1
    Iterator Insert(ConstIterator pos, const Type& value) {
        // Напишите тело самостоятельно
        auto n = std::distance(begin(), Iterator(pos));
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            Type* temp_items = new Type[new_capacity];
            std::copy(&items_[0], &items_[size_], &temp_items[0]);
            capacity_ = new_capacity;
            items_ = temp_items;
        }
        std::copy_backward(&items_[n], &items_[size_], &items_[size_ + 1]);
        items_[n] = value;
        ++size_;

        return &items_[n];
    }

    Iterator Insert(ConstIterator pos, Type&& value) {
        // Напишите тело самостоятельно
        size_t n = std::distance(begin(), Iterator(pos));
        if (size_ == capacity_) {
            size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
            Type* temp_items = new Type[new_capacity];
            int counter = 0;
            for (size_t i = 0; i < size_; ++i) {
                temp_items[counter] = std::move(items_[i]);
                ++counter;
            }
            capacity_ = new_capacity;
            items_ = std::move(temp_items);
        }

        size_t counter = size_;

        for (size_t i = size_; i > n; --i) {
            items_[counter] = std::move(items_[i]);
            counter=i;
        }

        items_[n]=std::move(value);
        ++size_;

        return &items_[n];
    }

    // "Удаляет" последний элемент вектора. Вектор не должен быть пустым
    void PopBack() noexcept {
        if (size_ > 0)
            --size_;
    }

    // Удаляет элемент вектора в указанной позиции
    Iterator Erase(ConstIterator pos) {
        if (size_ == 0 || capacity_ == 0) {
            return nullptr;
        }
        if (size_ == 1) {
            --size_;
            return end();
        }
        Type* temp_items = std::move(items_);
        size_t index = std::distance(begin(), Iterator(pos));

        int counter = index;
        for (size_t i = index + 1; i < size_; ++i) {
            temp_items[counter] = std::move(temp_items[i]);
            counter = i;
        }

        items_ = std::move(temp_items);
        --size_;
        return &items_[index];
    }


    // Обменивает значение с другим вектором
    void swap(SimpleVector& other) noexcept {
        // Напишите тело самостоятельно
        std::swap(items_, other.items_);
        std::swap(size_, other.size_);
        std::swap(capacity_, other.capacity_);
    }

    // Возвращает количество элементов в массиве
    size_t GetSize() const noexcept {
        // Напишите тело самостоятельно
        return size_;
    }

    // Возвращает вместимость массива
    size_t GetCapacity() const noexcept {
        // Напишите тело самостоятельно
        return capacity_;
    }

    // Сообщает, пустой ли массив
    bool IsEmpty() const noexcept {
        return size_ == 0;
    }

    // Возвращает ссылку на элемент с индексом index
    Type& operator[](size_t index) noexcept {
        // Напишите тело самостоятельно
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    const Type& operator[](size_t index) const noexcept {
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    Type& At(size_t index) {
        if (index >= size_)
            throw std::out_of_range("Out of range");
        return items_[index];
    }

    // Возвращает константную ссылку на элемент с индексом index
    // Выбрасывает исключение std::out_of_range, если index >= size
    const Type& At(size_t index) const {
        if (index >= size_)
            throw std::out_of_range("Out of range");
        return items_[index];
    }

    // Обнуляет размер массива, не изменяя его вместимость
    void Clear() noexcept {
        // Напишите тело самостоятельно
        size_ = 0;
    }

    // Изменяет размер массива.
    // При увеличении размера новые элементы получают значение по умолчанию для типа Type
    void Resize(size_t new_size) {
        // Напишите тело самостоятельно
        if (new_size <= size_) {
            size_ = new_size;
        }
        if (new_size > size_) {
            if (new_size < capacity_) {
                for (size_t i = size_; i < new_size; ++i) {
                    items_[i] = std::move(Type());
                }
                size_ = std::move(new_size);
            }
            if (new_size > capacity_) {
                size_t new_capacity = new_size;
                Type* temp_items = new Type[new_capacity];
                size_t counter = 0;
                for (size_t i = 0; i < size_; ++i) {
                    temp_items[counter] = std::move(items_[i]);
                    ++counter;
                }
                for (size_t i = size_; i < new_size; ++i) {
                    temp_items[i] = std::move(Type());
                }
                delete[] items_;
                items_ = std::move(temp_items);
                size_ = std::move(new_size);
                capacity_ = std::move(new_capacity);
            }
        }
    }

    // Возвращает итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator begin() noexcept {
        if (size_ == 0)
            return nullptr;
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    Iterator end() noexcept {
        // Напишите тело самостоятельно
        if (size_ == 0)
            return nullptr;
        return &items_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator begin() const noexcept {
        if (size_ == 0)
            return nullptr;
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator end() const noexcept {
        if (size_ == 0)
            return nullptr;
        return &items_[size_];
    }

    // Возвращает константный итератор на начало массива
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cbegin() const noexcept {
        if (size_ == 0)
            return nullptr;
        return &items_[0];
    }

    // Возвращает итератор на элемент, следующий за последним
    // Для пустого массива может быть равен (или не равен) nullptr
    ConstIterator cend() const noexcept {
        if (size_ == 0)
            return nullptr;
        return &items_[size_];
    }

private:
    Type* items_;
    size_t size_;
    size_t capacity_;
};

template <typename Type>
inline bool operator==(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::equal(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator!=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(lhs == rhs);
}

template <typename Type>
inline bool operator<(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <typename Type>
inline bool operator<=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(rhs < lhs);
}

template <typename Type>
inline bool operator>(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return rhs < lhs;
}

template <typename Type>
inline bool operator>=(const SimpleVector<Type>& lhs, const SimpleVector<Type>& rhs) {
    // Заглушка. Напишите тело самостоятельно
    return !(rhs > lhs);
}
ReserveProxyObj Reserve(size_t capacity_to_reserve) {
    return ReserveProxyObj(capacity_to_reserve);
}
