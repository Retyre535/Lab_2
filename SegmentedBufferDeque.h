#pragma once
#ifndef DEQUE_H
#define DEQUE_H

#include "Sequences.h"

template <typename T>
class Deque {
private:
    SegmentedList<T> buffer;

public:
    Deque() = default;

    Deque(T* items, int count) : buffer(items, count) {}

    Deque(const Deque<T>& other) : buffer(other.buffer) {}

    ~Deque() = default;

    Deque& operator=(const Deque& other) {
        if (this != &other) {
            buffer = other.buffer;
        }
        return *this;
    }

    bool IsEmpty() const {
        return buffer.GetSize() == 0;
    }

    int GetSize() const {
        return buffer.GetSize();
    }

    void PushFront(const T& item) {
        buffer.Prepend(item);
    }

    void PushBack(const T& item) {
        buffer.Append(item);
    }

    T PopFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = buffer.GetFirst();
        Sequence<T>* newSequence = buffer.GetSubSequence(1, buffer.GetSize() - 1);
        buffer = *dynamic_cast<SegmentedList<T>*>(newSequence);
        delete newSequence;
        return item;
    }

    T PopBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = buffer.GetLast();
        Sequence<T>* newSequence = buffer.GetSubSequence(0, buffer.GetSize() - 2);
        buffer = *dynamic_cast<SegmentedList<T>*>(newSequence);
        delete newSequence;
        return item;
    }

    T PeekFront() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return buffer.GetFirst();
    }

    T PeekBack() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return buffer.GetLast();
    }

    T Get(int index) const {
        return buffer.Get(index);
    }

    T& operator[](int index) {
        return buffer[index];
    }

    const T& operator[](int index) const {
        return buffer[index];
    }

    Deque<T> GetSubDeque(int startIndex, int endIndex) {
        Sequence<T>* subSequence = buffer.GetSubSequence(startIndex, endIndex);
        Deque<T> result;
        result.buffer = *dynamic_cast<SegmentedList<T>*>(subSequence);
        delete subSequence;
        return result;
    }

    Deque<T> Concat(const Deque<T>& other) const {
        Sequence<T>* concatenated = buffer.Concat(&other.buffer);
        Deque<T> result;
        result.buffer = *dynamic_cast<SegmentedList<T>*>(concatenated);
        delete concatenated;
        return result;
    }

    Sequence<T>* ToSequence() const {
        return buffer.Clone();
    }
};

#endif // DEQUE_H