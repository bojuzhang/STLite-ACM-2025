#ifndef SJTU_PRIORITY_QUEUE_HPP
#define SJTU_PRIORITY_QUEUE_HPP

#include <cstddef>
#include <functional>
#include "exceptions.hpp"

namespace sjtu {
/**
 * @brief a container like std::priority_queue which is a heap internal.
 * **Exception Safety**: The `Compare` operation might throw exceptions for certain data.
 * In such cases, any ongoing operation should be terminated, and the priority queue should be restored to its original state before the operation began.
 */
template<typename T, class Compare = std::less<T>>
class priority_queue {
private:
    struct heapnode {
        T val;
        struct heapnode *ls;
        struct heapnode *rs;
        // heapnode() : ls(nullptr), rs(nullptr) {};
        heapnode(const T &v) : val(v), ls(nullptr), rs(nullptr) {};
    };
    heapnode *root_;
    size_t size_;
    heapnode *merge(heapnode *x, heapnode *y) {
        if (!x) return y;
        if (!y) return x;
        if (Compare()(x->val, y->val)) {
            auto tmp = x;
            x = y;
            y = tmp;
        }
        auto tmp = x->rs;
        x->rs = x->ls;
        x->ls = merge(tmp, y);
        return x;
    }
    void clear(heapnode *x) {
        if (!x) return;
        clear(x->ls), clear(x->rs);
        delete x;
    }
    heapnode *clonenode(heapnode *other) {
        if (!other) return nullptr;
        heapnode *rt = new heapnode(other->val);
        rt->ls = clonenode(other->ls);
        rt->rs = clonenode(other->rs);
        return rt;
    }
public:
    /**
     * @brief default constructor
     */
    priority_queue() {
        size_ = 0;
        root_ = nullptr;
    }

    /**
     * @brief copy constructor
     * @param other the priority_queue to be copied
     */
    priority_queue(const priority_queue &other) {
        root_ = clonenode(other.root_);
        size_ = other.size_;
    }

    /**
     * @brief deconstructor
     */
    ~priority_queue() {
        clear(root_);
    }

    /**
     * @brief Assignment operator
     * @param other the priority_queue to be assigned from
     * @return a reference to this priority_queue after assignment
     */
    priority_queue &operator=(const priority_queue &other) {
        if (this == &other) {
            return *this;
        }
        clear(root_);
        root_ = clonenode(other.root_);
        size_ = other.size_;
        return *this;
    }

    /**
     * @brief get the top element of the priority queue.
     * @return a reference of the top element.
     * @throws container_is_empty if empty() returns true
     */
    const T & top() const {
        if (size_ == 0) {
            throw container_is_empty();
        }
        return root_->val;
    }

    /**
     * @brief push new element to the priority queue.
     * @param e the element to be pushed
     */
    void push(const T &e) {
        heapnode *newnode = new heapnode(e);
        try {
            root_ = merge(root_, newnode);
            ++size_;
        } catch(...) {
            delete newnode;
            throw ;
        }
    }

    /**
     * @brief delete the top element from the priority queue.
     * @throws container_is_empty if empty() returns true
     */
    void pop() {
        if (size_ == 0) {
            throw container_is_empty();
        }
        heapnode *tmp = root_;
        root_ = merge(root_->ls, root_->rs);
        delete tmp;
        --size_;
    }

    /**
     * @brief return the number of elements in the priority queue.
     * @return the number of elements.
     */
    size_t size() const {
        return size_;
    }

    /**
     * @brief check if the container is empty.
     * @return true if it is empty, false otherwise.
     */
    bool empty() const {
        return size_ == 0;
    }

    /**
     * @brief merge another priority_queue into this one.
     * The other priority_queue will be cleared after merging.
     * The complexity is at most O(logn).
     * @param other the priority_queue to be merged.
     */
    void merge(priority_queue &other) {
        if (this == &other) {
            return ;
        }
        // Thanks for sword_k_w for discussion about the case that comparision error emerges.
        try {
            Compare()(root_->val, other.root_->val);

            root_ = merge(root_, other.root_);
            size_ += other.size_;
            other.root_ = nullptr;
            other.size_ = 0;
        } catch (...) {
            throw ;
        }
    }
};

}

#endif