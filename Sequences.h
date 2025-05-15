#ifndef SEQUENCES_H
#define SEQUENCES_H

#include <iostream>
#include <functional>

const int AdaptiveSequenceSwitch = 200;

using namespace std;

class IndexOutOfRange : public exception {
public:
    const char* what() const noexcept override {
        return "Index out of range";
    }
};

template <class T>
class ICollection {
public:
    virtual ~ICollection() = default;
    virtual T Get(int index) const = 0;
    virtual int GetSize() const = 0;
    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void Insert(T item, int index) = 0;

};

template <class T>
class DynamicArray : public ICollection<T> {
private:
    T* data;
    int size;

public:

    DynamicArray() {
        size = 0;
        data = NULL;
    }

    DynamicArray(T* items, int count) : size(count) {
        data = new T[count];
        for (int i = 0; i < count; i++) {
            data[i] = items[i];
        }
    }

    DynamicArray(int size) : size(size) {
        if (size > 0) {
            data = new T[size];
        }
        else {
            data = 0;
        }
    }

    DynamicArray(DynamicArray<T>& dynamicArray) : size(dynamicArray.size()) {
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = dynamicArray[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    int GetSize() const override {
        return size;
    }

    T Get(int index) const override {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange();
        }
        return data[index];
    }

    void Set(int index, T value) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange();
        }
        data[index] = value;
    }

    void Resize(int newSize) {
        T* newData = new T[newSize];
        int copySize = (newSize < size) ? newSize : size;
        if (copySize < 0) {
            throw IndexOutOfRange();
        }
        for (int i = 0; i < copySize; i++) {
            newData[i] = data[i];
        }
        delete[] data;
        data = newData;
        size = newSize;
    }

    T& operator[](int index) {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange();
        }
        return data[index];
    }

    const T& operator[](int index) const {
        if (index < 0 || index >= size) {
            throw IndexOutOfRange();
        }
        return data[index];
    }

    void Append(T item) override {
        Resize(GetSize() + 1);
        data[GetSize() - 1] = item;
    }

    void Prepend(T item) override {
        Resize(GetSize() + 1);
        for (int i = GetSize() - 1; i >= 0; i--) {
            Set(i, item);
        }
        Set(0, item);
    }

    void Insert(T item, int index) override {
        if (index < 0 || index > size) {
            throw IndexOutOfRange();
        }

        if (index == 0) {
            Prepend(item);
        }
        else if (index == size) {
            Append(item);
        }
        else {
            Resize(GetSize() + 1);
            for (int i = GetSize() - 1; i >= index; i--) {
                Set(i, item);
            }
            Set(index, item);
        }
    }
};

template <class T>
class LinkedList : public ICollection<T> {
private:
    struct Node {
        T data;
        Node* next;
        Node(T data, Node* next = nullptr) : data(data), next(next) {}
    };
    Node* head;
    Node* tail;
    int size;

    Node* GetNode(int index) const {
        if (index < 0 || index >= size)
            throw IndexOutOfRange();
        Node* current = head;
        for (int i = 0; i < index; ++i)
            current = current->next;
        return current;
    }

public:
    LinkedList() : head(nullptr), tail(nullptr), size(0) {}

    LinkedList(T* items, int count) : LinkedList() {
        for (int i = 0; i < count; i++) {
            Append(items[i]);
        }
    }

    LinkedList(LinkedList<T>& list) : LinkedList() {
        Node* current = list.head;
        while (current != nullptr) {
            Append(current->data);
            current = current->next;
        }
    }

    ~LinkedList() {
        while (head != nullptr) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    T GetFirst() {
        if (head == nullptr) {
            throw IndexOutOfRange();
        }
        return head->data;
    }

    T GetLast() {
        if (tail == nullptr) {
            throw IndexOutOfRange();
        }
        return tail->data;
    }

    T Get(int index) const override {
        return GetNode(index)->data;
    }

    T& operator[](int index) {
        return GetNode(index)->data;
    }

    const T& operator[](int index) const {
        return GetNode(index)->data;
    }

    int GetSize() const override {
        return size;
    }

    LinkedList<T>* GetSubList(int startIndex, int endIndex) {
        if (startIndex < 0 || endIndex >= size || startIndex > endIndex) {
            throw IndexOutOfRange();
        }
        LinkedList<T>* subList = new LinkedList<T>();
        for (int i = startIndex; i <= endIndex; i++) {
            subList->Append(Get(i));
        }
        return subList;
    }

    void Append(T item) override {
        Node* newNode = new Node(item);
        if (head == nullptr) {
            head = tail = newNode;
        }
        else {
            tail->next = newNode;
            tail = newNode;
        }
        size++;
    }

    void Prepend(T item) override {
        head = new Node(item, head);
        if (tail == nullptr) {
            tail = head;
        }
        size++;
    }

    void Insert(T item, int index) override {
        if (index < 0 || index > size) {
            throw IndexOutOfRange();
        }

        if (index == 0) {
            Prepend(item);
        }
        else if (index == size) {
            Append(item);
        }
        else {
            Node* oldNode = GetNode(index - 1);
            Node* newNode = new Node(item, oldNode->next);
            oldNode->next = newNode;
            size++;
        }
    }

    LinkedList<T>* Concat(LinkedList<T>* list) {
        LinkedList<T>* newList = new LinkedList<T>(*this);
        Node* current = list->head;
        while (current != nullptr) {
            newList->Append(current->data);
            current = current->next;
        }
        return newList;
    }
};

template <class T>
class Sequence : public ICollection<T> {
public:
    virtual ~Sequence() = default;
    virtual T GetFirst() = 0;
    virtual T GetLast() = 0;
    virtual T Get(int index) const = 0;
    virtual Sequence<T>* Instance() = 0;
    virtual Sequence<T>* Clone() const = 0;
    virtual Sequence<T>* GetSubSequence(int startIndex, int endIndex) = 0;
    virtual int GetSize() const = 0;
    virtual void Append(T item) = 0;
    virtual void Prepend(T item) = 0;
    virtual void Insert(T item, int index) = 0;
    virtual Sequence<T>* Concat(Sequence<T>* list) = 0;
    virtual Sequence<T>* Map(function<T(T)> func) = 0;
    virtual T Reduce(function<T(T, T)> func, T startValue) = 0;
    virtual Sequence<T>* Where(function<bool(T)> predicate) = 0;
    virtual Sequence<T>* Zip(Sequence<T>* other, function<T(T, T)> func) = 0;
    virtual Sequence<T>* Slice(int index, int count, Sequence<T>* replacement) = 0;
    virtual Sequence<T>* Split(function<bool(T)> predicate) = 0;
    virtual bool TryGet(int index, T& value) = 0;
    virtual bool TryFind(function<bool(T)> predicate, T& value) = 0;
    virtual T& operator[](int index) = 0;
    virtual const T& operator[](int index) const = 0;
};

template <class T>
class ArraySequence : public Sequence<T> {
protected:
    DynamicArray<T>* array;
public:
    ArraySequence(T* items, int count) {
        array = new DynamicArray<T>(items, count);
    }

    ArraySequence() {
        array = new DynamicArray<T>(0);
    }

    ArraySequence(ArraySequence<T>& cArray) {
        array = new DynamicArray<T>(cArray.GetSize());
        for (int i = 0; i < cArray.GetSize(); ++i) {
            array->Set(i, cArray.Get(i));
        }
    }

    ~ArraySequence() {
        delete array;
    }

    T GetFirst() override {
        if (array->GetSize() == 0) throw IndexOutOfRange();
        return array->Get(0);
    }

    T GetLast() override {
        if (array->GetSize() == 0) throw IndexOutOfRange();
        return array->Get(array->GetSize() - 1);
    }

    int GetSize() const override {
        return array->GetSize();
    }

    T Get(int index) const override {
        return array->Get(index);
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) override {
        if (startIndex < 0 || endIndex >= array->GetSize() || startIndex > endIndex) {
            throw IndexOutOfRange();
        }
        ArraySequence<T>* subSequence = new ArraySequence<T>();
        for (int i = startIndex; i <= endIndex; i++) {
            subSequence->Append(array->Get(i));
        }
        return subSequence;
    }

    void Append(T item) override {
        array->Append(item);
    }

    void Prepend(T item) override {
        array->Prepend(item);
    }

    void Insert(T item, int index) override {
        array->Insert(item, index);
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>(*this);
        for (int i = 0; i < list->GetSize(); ++i) {
            newSequence->Append(list->Get(i));
        }
        return newSequence;
    }

    Sequence<T>* Map(std::function<T(T)> func) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        for (int i = 0; i < array->GetSize(); ++i) {
            newSequence->Append(func(array->Get(i)));
        }
        return newSequence;
    }

    T Reduce(std::function<T(T, T)> func, T startValue) override {
        T result = startValue;
        for (int i = 0; i < array->GetSize(); ++i) {
            result = func(result, array->Get(i));
        }
        return result;
    }

    Sequence<T>* Where(std::function<bool(T)> predicate) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        for (int i = 0; i < array->GetSize(); ++i) {
            if (predicate(array->Get(i))) {
                newSequence->Append(array->Get(i));
            }
        }
        return newSequence;
    }

    Sequence<T>* Zip(Sequence<T>* other, std::function<T(T, T)> func) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        int minLength = std::min(array->GetSize(), other->GetSize());
        for (int i = 0; i < minLength; ++i) {
            newSequence->Append(func(array->Get(i), other->Get(i)));
        }
        return newSequence;
    }

    Sequence<T>* Slice(int index, int count, Sequence<T>* replacement) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        if (index < 0) {
            index = array->GetSize() + index;
            if (index < 0) {
                throw IndexOutOfRange();
            }
        }
        if (index >= array->GetSize() || index + count > array->GetSize()) {
            throw IndexOutOfRange();
        }
        for (int i = 0; i < index; ++i) {
            newSequence->Append(array->Get(i));
        }
        if (replacement != nullptr) {
            for (int i = 0; i < replacement->GetSize(); ++i) {
                newSequence->Append(replacement->Get(i));
            }
        }
        for (int i = index + count; i < array->GetSize(); ++i) {
            newSequence->Append(array->Get(i));
        }
        return newSequence;
    }

    Sequence<T>* Split(std::function<bool(T)> predicate) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        ArraySequence<T>* currentChunk = new ArraySequence<T>();
        for (int i = 0; i < array->GetSize(); ++i) {
            T item = array->Get(i);
            if (predicate(item)) {
                if (currentChunk->GetSize() > 0) {
                    newSequence->Append(currentChunk->Get(0)); // Simplified for example
                    delete currentChunk;
                    currentChunk = new ArraySequence<T>();
                }
            }
            else {
                currentChunk->Append(item);
            }
        }
        if (currentChunk->GetSize() > 0) {
            newSequence->Append(currentChunk->Get(0)); // Simplified for example
        }
        delete currentChunk;
        return newSequence;
    }

    bool TryGet(int index, T& value) override {
        if (index < 0 || index >= array->GetSize()) {
            return false;
        }
        value = array->Get(index);
        return true;
    }

    bool TryFind(std::function<bool(T)> predicate, T& value) override {
        for (int i = 0; i < array->GetSize(); i++) {
            if (predicate(array->Get(i))) {
                value = array->Get(i);
                return true;
            }
        }
        return false;
    }

    T& operator[](int index) override {
        return (*array)[index];
    }

    const T& operator[](int index) const override {
        return (*array)[index];
    }

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        ArraySequence<T>* clone = new ArraySequence<T>();
        for (int i = 0; i < this->GetSize(); ++i) {
            clone->Append(this->Get(i));
        }
        return clone;
    }
};

template <class T>
class MutableArraySequence {
public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Instance() {
        return this;
    }

    Sequence<T>* Clone() const {
        return new MutableArraySequence<T>(*this);
    }
};

template <class T>
class ImmutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    Sequence<T>* Instance() override {
        return this->Clone();
    }

    Sequence<T>* Clone() const override {
        return new ImmutableArraySequence<T>(*this);
    }
};

template <class T>
class ListSequence : public Sequence<T> {
protected:
    LinkedList<T>* list;
public:
    ListSequence(T* items, int count) {
        list = new LinkedList<T>(items, count);
    }

    ListSequence() {
        list = new LinkedList<T>();
    }

    ListSequence(const ListSequence<T>& other) {
        list = new LinkedList<T>();
        for (int i = 0; i < other.GetSize(); ++i) {
            list->Append(other.Get(i));
        }
    }

    ~ListSequence() {
        delete list;
    }

    T GetFirst() override {
        return list->GetFirst();
    }

    T GetLast() override {
        return list->GetLast();
    }

    T Get(int index) const override {
        return list->Get(index);
    }

    int GetSize() const override {
        return list->GetSize();
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) override {
        if (startIndex < 0 || endIndex >= GetSize() || startIndex > endIndex)
            throw IndexOutOfRange();

        ListSequence<T>* subSequence = new ListSequence<T>();
        for (int i = startIndex; i <= endIndex; ++i) {
            subSequence->Append(Get(i));
        }
        return subSequence;
    }

    void Append(T item) override {
        list->Append(item);
    }

    void Prepend(T item) override {
        list->Prepend(item);
    }

    void Insert(T item, int index) override {
        list->Insert(item, index);
    }

    Sequence<T>* Concat(Sequence<T>* other) override {
        ListSequence<T>* newSequence = new ListSequence<T>(*this);
        for (int i = 0; i < other->GetSize(); ++i) {
            newSequence->Append(other->Get(i));
        }
        return newSequence;
    }

    Sequence<T>* Map(std::function<T(T)> func) override {
        ListSequence<T>* newSequence = new ListSequence<T>();
        for (int i = 0; i < GetSize(); ++i) {
            newSequence->Append(func(Get(i)));
        }
        return newSequence;
    }

    T Reduce(std::function<T(T, T)> func, T startValue) override {
        T result = startValue;
        for (int i = 0; i < GetSize(); ++i) {
            result = func(result, Get(i));
        }
        return result;
    }

    Sequence<T>* Where(std::function<bool(T)> predicate) override {
        ListSequence<T>* newSequence = new ListSequence<T>();
        for (int i = 0; i < GetSize(); ++i) {
            T item = Get(i);
            if (predicate(item)) {
                newSequence->Append(item);
            }
        }
        return newSequence;
    }

    Sequence<T>* Zip(Sequence<T>* other, std::function<T(T, T)> func) override {
        ListSequence<T>* newSequence = new ListSequence<T>();
        int minLength = std::min(GetSize(), other->GetSize());
        for (int i = 0; i < minLength; ++i) {
            newSequence->Append(func(Get(i), other->Get(i)));
        }
        return newSequence;
    }

    Sequence<T>* Slice(int index, int count, Sequence<T>* replacement) override {
        ListSequence<T>* newSequence = new ListSequence<T>();

        if (index < 0) {
            index = GetSize() + index;
            if (index < 0) throw IndexOutOfRange();
        }

        if (index >= GetSize() || index + count > GetSize()) {
            throw IndexOutOfRange();
        }

        // Add elements before the slice
        for (int i = 0; i < index; ++i) {
            newSequence->Append(Get(i));
        }

        // Add replacement if provided
        if (replacement != nullptr) {
            for (int i = 0; i < replacement->GetSize(); ++i) {
                newSequence->Append(replacement->Get(i));
            }
        }

        // Add elements after the slice
        for (int i = index + count; i < GetSize(); ++i) {
            newSequence->Append(Get(i));
        }

        return newSequence;
    }

    Sequence<T>* Split(std::function<bool(T)> predicate) override {
        ListSequence<T>* result = new ListSequence<T>();
        ListSequence<T>* current = new ListSequence<T>();

        for (int i = 0; i < GetSize(); ++i) {
            T item = Get(i);
            if (predicate(item)) {
                if (current->GetSize() > 0) {
                    result->Append(current->Get(0)); // Simplified
                    delete current;
                    current = new ListSequence<T>();
                }
            }
            else {
                current->Append(item);
            }
        }

        if (current->GetSize() > 0) {
            result->Append(current->Get(0)); // Simplified
        }
        delete current;

        return result;
    }

    bool TryGet(int index, T& value) override {
        if (index < 0 || index >= GetSize()) {
            return false;
        }
        value = Get(index);
        return true;
    }

    bool TryFind(std::function<bool(T)> predicate, T& value) override {
        for (int i = 0; i < GetSize(); ++i) {
            if (predicate(Get(i))) {
                value = Get(i);
                return true;
            }
        }
        return false;
    }

    T& operator[](int index) override {
        return (*list)[index];
    }

    const T& operator[](int index) const override {
        return (*list)[index];
    }

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new ListSequence<T>(*this);
    }
};

template <class T>
class MutableListSequence : public ListSequence<T> {
    using ListSequence<T>::ListSequence;

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
    using ListSequence<T>::ListSequence;

    Sequence<T>* Instance() override {
        return this->Clone();
    }

    Sequence<T>* Clone() const override {
        return new ImmutableListSequence<T>(*this);
    }
};

template <class T>
class AdaptiveSequence : public Sequence<T> {
private:
    Sequence<T>* sequence;

    void SwitchToList() {
        Sequence<T>* newList = new ListSequence<T>();
        for (int i = 0; i < sequence->GetSize(); ++i)
            newList->Append(sequence->Get(i));
        delete sequence;
        sequence = newList;
    }

    void SwitchToArray() {
        Sequence<T>* newArray = new ArraySequence<T>();
        for (int i = 0; i < sequence->GetSize(); ++i)
            newArray->Append(sequence->Get(i));
        delete sequence;
        sequence = newArray;
    }

public:
    AdaptiveSequence() {
        sequence = new ArraySequence<T>();
    }

    AdaptiveSequence(T* items, int count) {
        sequence = new ArraySequence<T>(items, count);
    }

    AdaptiveSequence(const AdaptiveSequence<T>& other) {
        sequence = other.sequence->Clone();
    }

    ~AdaptiveSequence() {
        delete sequence;
    }

    T GetFirst() override {
        return sequence->GetFirst();
    }

    T GetLast() override {
        return sequence->GetLast();
    }

    T Get(int index) const override {
        return sequence->Get(index);
    }

    int GetSize() const override {
        return sequence->GetSize();
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) override {
        return sequence->GetSubSequence(startIndex, endIndex);
    }

    void Append(T item) override {
        if (dynamic_cast<ArraySequence<T>*>(sequence) && sequence->GetSize() > AdaptiveSequenceSwitch) {
            SwitchToList();
        }
        sequence->Append(item);
    }

    void Prepend(T item) override {
        if (dynamic_cast<ArraySequence<T>*>(sequence) && sequence->GetSize() > AdaptiveSequenceSwitch) {
            SwitchToList();
        }
        sequence->Prepend(item);
    }

    void Insert(T item, int index) override {
        if (dynamic_cast<ArraySequence<T>*>(sequence) && sequence->GetSize() > AdaptiveSequenceSwitch) {
            SwitchToList();
        }
        sequence->Insert(item, index);
    }

    Sequence<T>* Concat(Sequence<T>* other) override {
        return sequence->Concat(other);
    }

    Sequence<T>* Map(std::function<T(T)> func) override {
        return sequence->Map(func);
    }

    T Reduce(std::function<T(T, T)> func, T startValue) override {
        return sequence->Reduce(func, startValue);
    }

    Sequence<T>* Where(std::function<bool(T)> predicate) override {
        return sequence->Where(predicate);
    }

    Sequence<T>* Zip(Sequence<T>* other, std::function<T(T, T)> func) override {
        return sequence->Zip(other, func);
    }

    Sequence<T>* Slice(int index, int count, Sequence<T>* replacement) override {
        return sequence->Slice(index, count, replacement);
    }

    Sequence<T>* Split(std::function<bool(T)> predicate) override {
        return sequence->Split(predicate);
    }

    bool TryGet(int index, T& value) override {
        return sequence->TryGet(index, value);
    }

    bool TryFind(std::function<bool(T)> predicate, T& value) override {
        return sequence->TryFind(predicate, value);
    }

    T& operator[](int index) override {
        return (*sequence)[index];
    }

    const T& operator[](int index) const override {
        return (*sequence)[index];
    }

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new AdaptiveSequence<T>(*this);
    }
};

template <class T>
class SegmentedList : public ICollection<T> {
private:
    static const size_t SEGMENT_SIZE = 32;

    struct Segment {
        T data[SEGMENT_SIZE];
        size_t size = 0;
        Segment* next = nullptr;

        ~Segment() {
            delete next;
        }
    };

    Segment* head = nullptr;
    size_t totalSize = 0;

    pair<Segment*, size_t> GetSegment(size_t index) {
        if (index >= totalSize) {
            throw IndexOutOfRange();
        }
        Segment* current = head;
        while (index >= SEGMENT_SIZE && current->next) {
            index -= SEGMENT_SIZE;
            current = current->next;
        }
        return { current, index };
    }

public:
    ~SegmentedList() {
        delete head;
    }

    T Get(int index) const override {
        pair<Segment*, size_t> segmentInfo = GetSegment(index);
        return segmentInfo.first->data[segmentInfo.second];
    }

    int GetSize() const override {
        return totalSize;
    }

    void Append(T item) override {
        if (!head) {
            head = new Segment();
        }

        Segment* current = head;
        while (current->next) {
            current = current->next;
        }

        if (current->size == SEGMENT_SIZE) {
            current->next = new Segment();
            current = current->next;
        }

        current->data[current->size++] = item;
        totalSize++;
    }

    void Prepend(T item) override {
        if (!head) {
            head = new Segment();
        }
        if (head->size == SEGMENT_SIZE) {
            Segment* newSegment = new Segment();
            newSegment->next = head;
            head = newSegment;
        }
        for (size_t i = head->size; i > 0; --i) {
            head->data[i] = head->data[i - 1];
        }
        head->data[0] = item;
        head->size++;
        totalSize++;
    }

    void Insert(T item, int index) override {
        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == totalSize) {
            Append(item);
            return;
        }
        pair<Segment*, size_t> segmentInfo = GetSegment(index);
        Segment* segment = segmentInfo.first;
        size_t offset = segmentInfo.second;
        if (segment->size == SEGMENT_SIZE) {
            Segment* newSegment = new Segment();
            size_t moveCount = SEGMENT_SIZE / 2;
            size_t startIndex = SEGMENT_SIZE - moveCount;
            for (size_t i = 0; i < moveCount; ++i) {
                newSegment->data[i] = segment->data[startIndex + i];
            }
            newSegment->size = moveCount;
            segment->size -= moveCount;
            newSegment->next = segment->next;
            segment->next = newSegment;
            if (offset >= segment->size) {
                offset -= segment->size;
                segment = newSegment;
            }
        }
        for (size_t i = segment->size; i > offset; --i) {
            segment->data[i] = segment->data[i - 1];
        }
        segment->data[offset] = item;
        segment->size++;
        totalSize++;
    }
};

template <class T>
class Option {
private:
    T value;
    bool hasValue;
public:
    Option() : hasValue(false) {}
    Option(T val) : value(val), hasValue(true) {}
    static Option<T> None() {
        return Option<T>();
    }

    bool IsSome() {
        return hasValue;
    }

    bool IsNone() {
        return !hasValue;
    }

    T GetValue() {
        if (!hasValue) {
            throw runtime_error("Option is None");
        }
        return value;
    }
};

#endif //SEQUENCES_H