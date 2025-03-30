/*
 * MutablePriorityQueue.h
 * A simple implementation of mutable priority queues, required by Dijkstra algorithm.
 *
 * Created on: 17/03/2018
 *      Author: João Pascoal Faria
 */

#ifndef DA_TP_CLASSES_MUTABLEPRIORITYQUEUE
#define DA_TP_CLASSES_MUTABLEPRIORITYQUEUE

#include <vector>

/**
 * @brief A class template for a mutable priority queue.
 *
 * This queue allows for decreasing the priority of an element, which is essential for algorithms
 * such as Dijkstra's algorithm.
 *
 * @tparam T Type of the elements stored in the priority queue. The class `T` must have the following:
 *          1. An accessible field `int queueIndex`
 *          2. A defined `operator<` for comparison.
 */
template <class T>
class MutablePriorityQueue {
    std::vector<T *> H; ///< A vector storing the elements of the heap.

    /**
    * @brief Moves the element at index `i` upwards in the heap to restore heap properties.
    *
    * This operation is used when inserting an element or decreasing the key of an element.
    *
    * @param i Index of the element to move upwards.
    */
    void heapifyUp(unsigned i);

    /**
    * @brief Moves the element at index `i` downwards in the heap to restore heap properties.
    *
    * This operation is used after extracting the minimum element from the priority queue.
    *
    * @param i Index of the element to move downwards.
    */
    void heapifyDown(unsigned i);

    /**
    * @brief Sets the element at index `i` in the heap to the provided element.
    *
    * Also updates the `queueIndex` of the element to reflect its new position in the heap.
    *
    * @param i Index at which to set the element.
    * @param x Pointer to the element to set at the given index.
    */
    inline void set(unsigned i, T * x);
public:

    /**
    * @brief Constructor for the mutable priority queue.
    * Initializes an empty heap.
    */
    MutablePriorityQueue();

    /**
    * @brief Inserts a new element into the priority queue.
    *
    * The element is inserted at the end of the heap and then moved up to restore heap properties.
    *
    * @param x Pointer to the element to insert.
    */
    void insert(T * x);

    /**
    * @brief Extracts and returns the minimum element from the priority queue.
    *
    * After extracting the minimum, the last element in the heap is moved to the root, and the heap is
    * restructured to restore heap properties.
    *
    * @return Pointer to the extracted minimum element.
    */
    T * extractMin();

    /**
    * @brief Decreases the key (priority) of the given element in the priority queue.
    *
    * This is done by moving the element upwards in the heap.
    *
    * @param x Pointer to the element whose priority needs to be decreased.
    */
    void decreaseKey(T * x);

    /**
    * @brief Checks if the priority queue is empty.
    *
    * @return `true` if the queue is empty, otherwise `false`.
    */
    bool empty();
};

// Index calculations
#define parent(i) ((i) / 2) ///< Index of the parent of the element at index `i`.
#define leftChild(i) ((i) * 2) ///< Index of the left child of the element at index `i`.

template <class T>
MutablePriorityQueue<T>::MutablePriorityQueue() {
    H.push_back(nullptr); ///< Initialize the heap with a placeholder for index 0.
    // Indices will be used starting from 1 to simplify parent/child calculations.
    // indices will be used starting in 1
    // to facilitate parent/child calculations
}


template <class T>
bool MutablePriorityQueue<T>::empty() {
    return H.size() == 1;
}

template <class T>
T* MutablePriorityQueue<T>::extractMin() {
    auto x = H[1];
    H[1] = H.back();
    H.pop_back();
    if(H.size() > 1) heapifyDown(1);
    x->setQueueIndex(0);
    return x;
}

template <class T>
void MutablePriorityQueue<T>::insert(T *x) {
    H.push_back(x);
    heapifyUp(H.size()-1);
}

template <class T>
void MutablePriorityQueue<T>::decreaseKey(T *x) {
    heapifyUp(x->getQueueIndex());
}

template <class T>
void MutablePriorityQueue<T>::heapifyUp(unsigned i) {
    auto x = H[i];
    while (i > 1 && *x < *H[parent(i)]) {
        set(i, H[parent(i)]);
        i = parent(i);
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::heapifyDown(unsigned i) {
    auto x = H[i];
    while (true) {
        unsigned k = leftChild(i);
        if (k >= H.size())
            break;
        if (k+1 < H.size() && *H[k+1] < *H[k])
            ++k; // right child of i
        if ( ! (*H[k] < *x) )
            break;
        set(i, H[k]);
        i = k;
    }
    set(i, x);
}

template <class T>
void MutablePriorityQueue<T>::set(unsigned i, T * x) {
    H[i] = x;
    x->setQueueIndex(i);
}

#endif /* DA_TP_CLASSES_MUTABLEPRIORITYQUEUE */