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
class Optional {
private:
    T value;
    bool hasValue;
public:
    Optional() : hasValue(false) {}
    Optional(T val) : value(val), hasValue(true) {}
    static Optional<T> None() {
        return Optional<T>();
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

template <class T>
class DynamicArray {
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

    DynamicArray(DynamicArray<T>& dynamicArray) : size(dynamicArray.GetSize()) {
        data = new T[size];
        for (int i = 0; i < size; i++) {
            data[i] = dynamicArray[i];
        }
    }

    ~DynamicArray() {
        delete[] data;
    }

    int GetSize() const {
        return size;
    }

    T Get(int index) const {
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

    void Append(T item) {
        Resize(GetSize() + 1);
        data[GetSize() - 1] = item;
    }

    void Prepend(T item) {
        Resize(GetSize() + 1);
        for (int i = GetSize() - 1; i > 0; --i) {
            data[i] = data[i - 1];
        }
        data[0] = item;
    }

    void Insert(T item, int index) {
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
            for (int i = GetSize() - 1; i > index; i--) {
                data[i] = data[i - 1];
            }
            data[index] = item;
        }
    }
};

template <class T>
class LinkedList {
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

    T Get(int index) const {
        return GetNode(index)->data;
    }

    T& operator[](int index) {
        return GetNode(index)->data;
    }

    const T& operator[](int index) const {
        return GetNode(index)->data;
    }

    int GetSize() const {
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

    void Append(T item) {
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

    void Prepend(T item) {
        head = new Node(item, head);
        if (tail == nullptr) {
            tail = head;
        }
        size++;
    }

    void Insert(T item, int index) {
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
    virtual Sequence<T>* From(const Sequence<T>& other) = 0;
    virtual Sequence<T>* Zip(const Sequence<T>& other) const = 0;
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

    ArraySequence() {
        array = new DynamicArray<T>(0);
    }

    ArraySequence(T* items, int count) {
        array = new DynamicArray<T>(items, count);
    }

    ArraySequence(const ArraySequence<T>& other) {
        array = new DynamicArray<T>(*other.array);
    }

    ArraySequence(const Sequence<T>& other) {
        array = new DynamicArray<T>(0);
        for (int i = 0; i < other.GetSize(); ++i) {
            array->Append(other.Get(i));
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

    Sequence<T>* Map(function<T(T)> func) override {
        ArraySequence<T>* newSequence = new ArraySequence<T>();
        for (int i = 0; i < array->GetSize(); ++i) {
            newSequence->Append(func(array->Get(i)));
        }
        return newSequence;
    }

    Sequence<T>* From(const Sequence<T>& other) override {
        Sequence<T>* result = new ArraySequence<T>();
        for (int i = 0; i < other.GetSize(); ++i) {
            result->Append(other.Get(i));
        }
        return result;
    }

    Sequence<T>* Zip(const Sequence<T>& other) const override {
        ArraySequence<T>* result = new ArraySequence<T>();
        int minSize = min(this->GetSize(), other.GetSize());
        for (int i = 0; i < minSize; ++i) {
            result->Append(this->Get(i));
            result->Append(other.Get(i));
        }
        return result;
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
class MutableArraySequence : public ArraySequence<T> {
public:
    using ArraySequence<T>::ArraySequence;

    MutableArraySequence() : ArraySequence<T>() {}

    MutableArraySequence(T* items, int count) : ArraySequence<T>(items, count) {}

    MutableArraySequence(const MutableArraySequence<T>& other) : ArraySequence<T>(other) {}

    MutableArraySequence(const Sequence<T>& other) : ArraySequence<T>(other) {}

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

    ImmutableArraySequence(const ImmutableArraySequence<T>& other) : ArraySequence<T>(other) {}

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
    ListSequence(T* items, int count) : list(new LinkedList<T>(items, count)) {}

    ListSequence() {
        list = new LinkedList<T>();
    }

    ListSequence(const Sequence<T>& other) : list(new LinkedList<T>()) {
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

    Sequence<T>* Map(function<T(T)> func) override {
        ListSequence<T>* newSequence = new ListSequence<T>();
        for (int i = 0; i < GetSize(); ++i) {
            newSequence->Append(func(Get(i)));
        }
        return newSequence;
    }

    Sequence<T>* From(const Sequence<T>& other) override {
        Sequence<T>* result = new ListSequence<T>();
        for (int i = 0; i < other.GetSize(); ++i) {
            result->Append(other.Get(i));
        }
        return result;
    }

    Sequence<T>* Zip(const Sequence<T>& other) const override {
        ListSequence<T>* result = new ListSequence<T>();
        int minSize = min(this->GetSize(), other.GetSize());
        for (int i = 0; i < minSize; ++i) {
            result->Append(this->Get(i));
            result->Append(other.Get(i));
        }
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
public:
    using ListSequence<T>::ListSequence;
    MutableListSequence() : ListSequence<T>() {}

    MutableListSequence(T* items, int count) : ListSequence<T>(items, count) {}

    MutableListSequence(const MutableListSequence<T>& other) : ListSequence<T>(other) {}

    MutableListSequence(const Sequence<T>& other) : ListSequence<T>(other) {}

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new MutableListSequence<T>(*this);
    }
};

template <class T>
class ImmutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;

    ImmutableListSequence(const ImmutableListSequence<T>& other) : ListSequence<T>(other) {}

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

    Sequence<T>* Map(function<T(T)> func) override {
        return sequence->Map(func);
    }

    Sequence<T>* From(const Sequence<T>& other) override {
        AdaptiveSequence<T>* result = new AdaptiveSequence<T>();
        for (int i = 0; i < other.GetSize(); ++i) {
            result->Append(other.Get(i));
        }
        return result;
    }

    Sequence<T>* Zip(const Sequence<T>& other) const override {
        AdaptiveSequence<T>* result = new AdaptiveSequence<T>();
        int minSize = min(this->GetSize(), other.GetSize());
        for (int i = 0; i < minSize; ++i) {
            result->Append(this->Get(i));
            result->Append(other.Get(i));
        }
        return result;
    }

    bool TryGet(int index, T& value) override {
        return sequence->TryGet(index, value);
    }

    bool TryFind(function<bool(T)> predicate, T& value) override {
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
class SegmentedList : public Sequence<T> {
private:
    static const size_t SEGMENT_SIZE = 32;
    LinkedList<DynamicArray<T>*> segments;

    pair<DynamicArray<T>*, int> GetSegment(int index) const {
        if (index < 0 || index >= GetSize()) throw IndexOutOfRange();

        int currentPos = 0;
        for (int i = 0; i < segments.GetSize(); i++) {
            DynamicArray<T>* segment = segments.Get(i);
            if (index < currentPos + segment->GetSize()) {
                return { segment, index - currentPos };
            }
            currentPos += segment->GetSize();
        }
        throw IndexOutOfRange();
    }

    int GetSegmentIndex(int elementIndex) const {
        int currentPos = 0;
        for (int i = 0; i < segments.GetSize(); i++) {
            currentPos += segments.Get(i)->GetSize();
            if (elementIndex < currentPos) {
                return i;
            }
        }
        return segments.GetSize() - 1;
    }

    void SplitSegment(DynamicArray<T>* segment, int segmentIndex, int posInSegment) {
        DynamicArray<T>* newSegment = new DynamicArray<T>();
        int elementsToMove = segment->GetSize() - posInSegment;

        for (int i = 0; i < elementsToMove; i++) {
            newSegment->Append(segment->Get(posInSegment + i));
        }

        segment->Resize(posInSegment);
        segments.Insert(newSegment, segmentIndex + 1);
    }

public:

    SegmentedList() = default;

    SegmentedList(T* items, int count) {
        for (int i = 0; i < count; i += SEGMENT_SIZE) {
            int segmentSize = std::min(static_cast<int>(SEGMENT_SIZE), count - i);
            DynamicArray<T>* segment = new DynamicArray<T>(segmentSize);
            for (int j = 0; j < segmentSize; j++) {
                (*segment)[j] = items[i + j];
            }
            segments.Append(segment);
        }
    }

    SegmentedList(const SegmentedList<T>& other) {
        for (int i = 0; i < other.GetSize(); i++) {
            this->Append(other.Get(i));
        }
    }

    ~SegmentedList() {
        for (int i = 0; i < segments.GetSize(); i++) {
            delete segments.Get(i);
        }
    }

    T GetFirst() override {
        if (segments.GetSize() == 0) throw IndexOutOfRange();
        DynamicArray<T>* firstSegment = segments.Get(0);
        if (firstSegment->GetSize() == 0) throw IndexOutOfRange();
        return (*firstSegment)[0];
    }

    T GetLast() override {
        if (segments.GetSize() == 0) throw IndexOutOfRange();
        DynamicArray<T>* lastSegment = segments.Get(segments.GetSize() - 1);
        if (lastSegment->GetSize() == 0) throw IndexOutOfRange();
        return (*lastSegment)[lastSegment->GetSize() - 1];
    }

    T Get(int index) const override {
        auto segmentInfo = GetSegment(index);
        return segmentInfo.first->Get(segmentInfo.second);
    }

    int GetSize() const override {
        int size = 0;
        for (int i = 0; i < segments.GetSize(); i++) {
            size += segments.Get(i)->GetSize();
        }
        return size;
    }

    void Append(T item) override {
        if (segments.GetSize() == 0 || segments.GetLast()->GetSize() >= SEGMENT_SIZE) {
            segments.Append(new DynamicArray<T>());
        }
        segments.GetLast()->Append(item);
    }

    void Prepend(T item) override {
        if (segments.GetSize() == 0 || segments.GetFirst()->GetSize() >= SEGMENT_SIZE) {
            segments.Prepend(new DynamicArray<T>());
        }
        segments.GetFirst()->Prepend(item);
    }

    void Insert(T item, int index) override {
        if (index < 0 || index > GetSize()) {
            throw IndexOutOfRange();
        }

        if (index == 0) {
            Prepend(item);
            return;
        }
        if (index == GetSize()) {
            Append(item);
            return;
        }

        auto segmentInfo = GetSegment(index);
        DynamicArray<T>* segment = segmentInfo.first;
        int posInSegment = segmentInfo.second;

        if (segment->GetSize() < SEGMENT_SIZE) {
            segment->Insert(item, posInSegment);
            return;
        }

        DynamicArray<T>* newSegment = new DynamicArray<T>();
        int splitPos = SEGMENT_SIZE / 2;

        for (int i = splitPos; i < segment->GetSize(); ++i) {
            newSegment->Append(segment->Get(i));
        }

        segment->Resize(splitPos);
        int segmentIndex = GetSegmentIndex(index);
        segments.Insert(newSegment, segmentIndex + 1);

        if (posInSegment >= splitPos) {
            newSegment->Insert(item, posInSegment - splitPos);
        }
        else {
            segment->Insert(item, posInSegment);
        }
    }

    Sequence<T>* GetSubSequence(int startIndex, int endIndex) override {
        SegmentedList<T>* subList = new SegmentedList<T>();
        for (int i = startIndex; i <= endIndex; i++) {
            subList->Append(this->Get(i));
        }
        return subList;
    }

    Sequence<T>* Concat(Sequence<T>* other) override {
        SegmentedList<T>* result = new SegmentedList<T>(*this);
        for (int i = 0; i < other->GetSize(); i++) {
            result->Append(other->Get(i));
        }
        return result;
    }

    Sequence<T>* Map(function<T(T)> func) override {
        SegmentedList<T>* result = new SegmentedList<T>();
        for (int i = 0; i < GetSize(); i++) {
            result->Append(func(this->Get(i)));
        }
        return result;
    }

    Sequence<T>* From(const Sequence<T>& other) {
        Sequence<T>* result = new SegmentedList<T>();
        for (int i = 0; i < other.GetSize(); ++i) {
            result->Append(other.Get(i));
        }
        return result;
    }

    Sequence<T>* Zip(const Sequence<T>& other) const override {
        SegmentedList<T>* result = new SegmentedList<T>();
        int minSize = min(this->GetSize(), other.GetSize());
        for (int i = 0; i < minSize; ++i) {
            result->Append(this->Get(i));
            result->Append(other.Get(i));
        }
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
        for (int i = 0; i < GetSize(); i++) {
            T item = Get(i);
            if (predicate(item)) {
                value = item;
                return true;
            }
        }
        return false;
    }

    T& operator[](int index) override {
        auto segmentInfo = GetSegment(index);
        return (*segmentInfo.first)[segmentInfo.second];
    }

    const T& operator[](int index) const override {
        auto segmentInfo = GetSegment(index);
        return (*segmentInfo.first)[segmentInfo.second];
    }

    Sequence<T>* Instance() override {
        return this;
    }

    Sequence<T>* Clone() const override {
        return new SegmentedList<T>(*this);
    }
};

#endif //SEQUENCES_H