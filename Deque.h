#pragma once
#ifndef DEQUE_H
#define DEQUE_H

#include "Sequences.h"

template <class T>
class Deque {
private:
    Sequence<T>* sequence;
public:
    Deque() : sequence(new ArraySequence<T>()) {}

    Deque(T* items, int count) : sequence(new ArraySequence<T>(items, count)) {}

    Deque(const Deque<T>& other) : sequence(other.sequence->Clone()) {}

    ~Deque() {
        delete sequence;
    }

    bool IsEmpty() const {
        return sequence->GetSize() == 0;
    }

    int GetSize() const {
        return sequence->GetSize();
    }

    void PushFront(T item) {
        sequence->Prepend(item);
    }

    void PushBack(T item) {
        sequence->Append(item);
    }

    T PopFront() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = sequence->GetFirst();
        Sequence<T>* newSequence = sequence->GetSubSequence(1, sequence->GetSize() - 1);
        delete sequence;
        sequence = newSequence;
        return item;
    }

    T PopBack() {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        T item = sequence->GetLast();
        Sequence<T>* newSequence = sequence->GetSubSequence(0, sequence->GetSize() - 2);
        delete sequence;
        sequence = newSequence;
        return item;
    }

    T PeekFront() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return sequence->GetFirst();
    }

    T PeekBack() const {
        if (IsEmpty()) {
            throw IndexOutOfRange();
        }
        return sequence->GetLast();
    }

    T Get(int index) const {
        return sequence->Get(index);
    }

    T& operator[](int index) {
        return (*sequence)[index];
    }

    const T& operator[](int index) const {
        return (*sequence)[index];
    }

    Deque<T>* GetSubDeque(int startIndex, int endIndex) {
        Sequence<T>* subSequence = sequence->GetSubSequence(startIndex, endIndex);
        Deque<T>* subDeque = new Deque<T>();
        delete subDeque->sequence;
        subDeque->sequence = subSequence;
        return subDeque;
    }

    Deque<T>* Concat(Deque<T>* other) {
        Sequence<T>* concatenated = sequence->Concat(other->sequence);
        Deque<T>* result = new Deque<T>();
        delete result->sequence;
        result->sequence = concatenated;
        return result;
    }
};

#endif // DEQUE_H