#ifndef SJTU_VECTOR_HPP
#define SJTU_VECTOR_HPP

#include "exceptions.hpp"

#include <climits>
#include <cstddef>
#include <cstdlib>

namespace sjtu {
/**
 * a data container like std::vector
 * store data in a successive memory and support random access.
 */
template<typename T>
class vector {
public:
    /**
     * a type for actions of the elements of a vector, and you should write
     *   a class named const_iterator with same interfaces.
     */
    /**
     * you can see RandomAccessIterator at CppReference for help.
     */
    class const_iterator;
    class iterator {
    // The following code is written for the C++ type_traits library.
    // Type traits is a C++ feature for describing certain properties of a type.
    // For instance, for an iterator, iterator::value_type is the type that the
    // iterator points to.
    // STL algorithms and containers may use these type_traits (e.g. the following
    // typedef) to work properly. In particular, without the following code,
    // @code{std::sort(iter, iter1);} would not compile.
    // See these websites for more information:
    // https://en.cppreference.com/w/cpp/header/type_traits
    // About value_type: https://blog.csdn.net/u014299153/article/details/72419713
    // About iterator_category: https://en.cppreference.com/w/cpp/iterator
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = T;
        using pointer = T*;
        using reference = T&;
        using iterator_category = std::output_iterator_tag;

    private:
        pointer ptr_;
        int idx_;
        friend class const_iterator;
    public:
        iterator() : ptr_(nullptr), idx_(0) { }
        iterator(pointer ptr, int idx) : ptr_(ptr), idx_(idx) { }
        
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        iterator operator+(const int &n) const {
            iterator p = *this;
            p.idx_ += n;
            return p;
        }
        iterator operator-(const int &n) const {
            iterator p = *this;
            p.idx_ += n;
            return p;
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const iterator &rhs) const {
            if (ptr_ != rhs.ptr_) {
                throw invalid_iterator();
            }
            return idx_ - rhs.idx_;
        }
        iterator& operator+=(const int &n) {
            idx_ += n;
            return *this;
        }
        iterator& operator-=(const int &n) {
            idx_ -= n;
            return *this;
        }
        /**
         * iter++
         */
        iterator operator++(int) {
            iterator p = *this;
            idx_++;
            return p;
        }
        /**
         * ++iter
         */
        iterator& operator++() {
            idx_++;
            return *this;
        }
        /**
         * iter--
         */
        iterator operator--(int) {
            iterator p = *this;
            idx_--;
            return p;
        }
        /**
         * --iter
         */
        iterator& operator--() {
            idx_--;
            return *this;
        }
        /**
         * *it
         */
        T& operator*() const {
            return ptr_[idx_];
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_ && idx_ == rhs.idx_;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr_ == rhs.ptr_ && idx_ == rhs.idx_;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return !((*this) == rhs);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !((*this) == rhs);
        }
    };
    /**
     * has same function as iterator, just for a const object.
     */
    class const_iterator {
    public:
        using difference_type = std::ptrdiff_t;
        using value_type = const T;
        using pointer = const T*;
        using reference = const T&;
        using iterator_category = std::output_iterator_tag;

    private:
        const pointer ptr_;
        int idx_;
        friend class iterator;
    public:
        const_iterator() : ptr_(nullptr), idx_(0) { }
        const_iterator(const pointer ptr, int idx) : ptr_(ptr), idx_(idx) { }
        /**
         * return a new iterator which pointer n-next elements
         * as well as operator-
         */
        const_iterator operator+(const int &n) const {
            const_iterator p = *this;
            p.idx_ += n;
            return p;
        }
        const_iterator operator-(const int &n) const {
            const_iterator p = *this;
            p.idx_ += n;
            return p;
        }
        // return the distance between two iterators,
        // if these two iterators point to different vectors, throw invaild_iterator.
        int operator-(const const_iterator &rhs) const {
            if (ptr_ != rhs.ptr_) {
                throw invalid_iterator();
            }
            return idx_ - rhs.idx_;
        }
        const_iterator& operator+=(const int &n) {
            idx_ += n;
            return *this;
        }
        const_iterator& operator-=(const int &n) {
            idx_ -= n;
            return *this;
        }
        /**
         * iter++
         */
        const_iterator operator++(int) {
            const_iterator p = *this;
            idx_++;
            return p;
        }
        /**
         * ++iter
         */
        const_iterator& operator++() {
            idx_++;
            return *this;
        }
        /**
         * iter--
         */
        const_iterator operator--(int) {
            const_iterator p = *this;
            idx_--;
            return p;
        }
        /**
         * --iter
         */
        const_iterator& operator--() {
            idx_--;
            return *this;
        }
        /**
         * *it
         */
        const T& operator*() const {
            return ptr_[idx_];
        }
        /**
         * a operator to check whether two iterators are same (pointing to the same memory address).
         */
        bool operator==(const iterator &rhs) const {
            return ptr_ == rhs.ptr_ && idx_ == rhs.idx_;
        }
        bool operator==(const const_iterator &rhs) const {
            return ptr_ == rhs.ptr_ && idx_ == rhs.idx_;
        }
        /**
         * some other operator for iterator.
         */
        bool operator!=(const iterator &rhs) const {
            return !((*this) == rhs);
        }
        bool operator!=(const const_iterator &rhs) const {
            return !((*this) == rhs);
        }
    };
    /**
     * TODO Constructs
     * At least two: default constructor, copy constructor
     */
    vector() { }
    vector(const vector &other) {
        *this = other;
    }
    /**
     * TODO Destructor
     */
    ~vector() {
        for (size_t i = 0; i < size_; i++) {
            data_[i].~T();
        }
        free(data_);
    }
    /**
     * TODO Assignment operator
     */
    vector &operator=(const vector &other) {
        if (this == &other) {
            return *this;
        }
        for (size_t i = 0; i < size_; i++) {
            data_[i].~T();
        }
        free(data_);
        size_ = other.size_;
        capacity_ = other.capacity_;
        data_ = reinterpret_cast<T*>(malloc(capacity_ * sizeof(T)));
        for (size_t i = 0; i < size_; i++) {
            new (data_ + i) T(other.data_[i]);
        }
        return *this;
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     */
    T & at(const size_t &pos) {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    const T & at(const size_t &pos) const {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    /**
     * assigns specified element with bounds checking
     * throw index_out_of_bound if pos is not in [0, size)
     * !!! Pay attentions
     *   In STL this operator does not check the boundary but I want you to do.
     */
    T & operator[](const size_t &pos) {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    const T & operator[](const size_t &pos) const {
        if (pos >= size_) {
            throw index_out_of_bound();
        }
        return data_[pos];
    }
    /**
     * access the first element.
     * throw container_is_empty if size == 0
     */
    const T & front() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
        return data_[0];
    }
    /**
     * access the last element.
     * throw container_is_empty if size == 0
     */
    const T & back() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
        return data_[size_ - 1];
    }
    /**
     * returns an iterator to the beginning.
     */
    iterator begin() {
        return iterator(data_, 0);
    }
    const_iterator begin() const {
        return const_iterator(data_, 0);
    }
    const_iterator cbegin() const {
        return const_iterator(const_cast<const T*>(data_), 0);
    }
    /**
     * returns an iterator to the end.
     */
    iterator end() {
        return iterator(data_, size_);
    }
    const_iterator end() const {
        return const_iterator(data_, size_);
    }
    const_iterator cend() const {
        return const_iterator(const_cast<typename const_iterator::pointer>(data_), size_);
    }
    /**
     * checks whether the container is empty
     */
    bool empty() const {
        return size_ == 0;
    }
    /**
     * returns the number of elements
     */
    size_t size() const {
        return size_;
    }
    size_t capacity() const {
        return capacity_;
    }
    /**
     * clears the contents
     */
    void clear() {
        for (size_t i = 0; i < size_; i++) {
            data_[i].~T();
        }
        size_ = 0;
    }
    /**
     * inserts value before pos
     * returns an iterator pointing to the inserted value.
     */
    iterator insert(iterator pos, const T &value) {
        return insert(pos - begin(), value);
    }
    /**
     * inserts value at index ind.
     * after inserting, this->at(ind) == value
     * returns an iterator pointing to the inserted value.
     * throw index_out_of_bound if ind > size (in this situation ind can be size because after inserting the size will increase 1.)
     */
    iterator insert(const size_t &ind, const T &value) {
        if (ind > size_) {
            throw index_out_of_bound();
        }
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }
        size_++;
        for (size_t i = size_ - 1; i > ind; i--) {
            new (data_ + i) T(std::move(data_[i - 1]));
        }
        new (data_ + ind) T(value);
        return iterator(data_, ind);
    }
    /**
     * removes the element at pos.
     * return an iterator pointing to the following element.
     * If the iterator pos refers the last element, the end() iterator is returned.
     */
    iterator erase(iterator pos) {
        return erase(pos - begin());
    }
    /**
     * removes the element with index ind.
     * return an iterator pointing to the following element.
     * throw index_out_of_bound if ind >= size
     */
    iterator erase(const size_t &ind) {
        if (ind >= size_) {
            throw index_out_of_bound();
        }
        data_[ind].~T();
        size_--;
        for (size_t i = ind; i < size_; i++) {
            new (data_ + i) T(std::move(data_[i + 1]));
        }
        return iterator(data_, ind);
    }
    /**
     * adds an element to the end.
     */
    void push_back(const T &value) {
        if (size_ == capacity_) {
            reserve(capacity_ ? capacity_ * 2 : 1);
        }
        new (data_ + size_) T(value); 
        ++size_;
    }
    /**
     * remove the last element from the end.
     * throw container_is_empty if size() == 0
     */
    void pop_back() {
        if (!size_) {
            throw container_is_empty();
        }
        size_--;
        data_[size_].~T();
    }

private:
    size_t size_ = 0;
    size_t capacity_ = 0;

    T* data_ = nullptr;

    void reserve(const size_t new_capacity) {
        capacity_ = new_capacity;
        T* new_data = reinterpret_cast<T*>(malloc(capacity_ * sizeof(T)));
        for (size_t i = 0; i < size_; i++) {
            new (new_data + i) T(std::move(data_[i]));
            data_[i].~T();
        }
        free(data_);
        data_ = new_data;

        // if (capacity_ < size_) {
        //     size_ = capacity_;
        // }
    }
};


}

#endif
