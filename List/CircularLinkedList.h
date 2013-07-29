#ifndef ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H
#define ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H

#include "List.h"

#include <initializer_list>
#include <memory>
#include <sstream>
#include <stdexcept>

namespace etsai {
namespace collections {
namespace list {

using collections::List;
using std::initializer_list;
using std::invalid_argument;
using std::out_of_range;
using std::shared_ptr;
using std::stringstream;

/**
 * Manages a circular linked list and implements the List interface
 * @author etsai
 */
template <class T>
class CircularLinkedList : public List<T> {
public:

    CircularLinkedList();
    CircularLinkedList(const T& defaultValue);
    CircularLinkedList(initializer_list<T> collection);
    CircularLinkedList(initializer_list<T> collection, const T& defaultValue);
    ~CircularLinkedList();
    
    virtual CircularLinkedList* clone() const;
    /**
     * Checks if both collection shave the same elements in the same order.  This version provides a way to quickly 
     * check the contents of a list
     * @param   collection      Collection to check again
     * @return  True if both collection shave the same elements and ordering
     */
    virtual bool equals(initializer_list<T> collection) const;
    /**
     * Checks if both collection shave the same elements in the same order
     * @param   collection      Collection to check again
     * @return  True if both collection shave the same elements and ordering
     */
    virtual bool equals(const Collection<T>* collection) const;
    virtual int size() const;
    virtual int capacity() const;
    virtual bool isEmpty() const;
    virtual bool contains(const T& elem) const;
    virtual string toString() const;
    virtual void each(const function<void (const T&)>& lambda) const;
    virtual void each(const function<void (T&)>& lambda);

    virtual bool remove(const T& elem); 
    virtual void add(const T& elem);
    virtual void clear();
    virtual void resize(int newSize);
    virtual void add(int index, const T& elem);
    virtual void set(int index, const T& elem) throw(out_of_range);
    virtual T minus(int index) throw(out_of_range);
    virtual T get(int index) const throw(out_of_range);
    virtual shared_ptr<List<T>> subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument);

private:
    template <class U>
    struct Node {
        U value;
        Node<U> *next;
    };
    
    int listSize;
    Node<T> *tail;
    T *defaultValue;
};

template <class T>
CircularLinkedList<T>::CircularLinkedList() : listSize(0), tail(NULL), defaultValue(NULL) {
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(const T& defaultValue) : CircularLinkedList() {
    this->defaultValue= new T(defaultValue);
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(initializer_list<T> collection) : CircularLinkedList() {
    for(auto &elem: collection) {
        add(elem);
    }
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(initializer_list<T> collection, const T& defaultValue) : CircularLinkedList(collection) {
    this->defaultValue= new T(defaultValue);
}

template <class T>
CircularLinkedList<T>::~CircularLinkedList() {
    clear();
    if (defaultValue != NULL) {
        delete defaultValue;
    }
}

template <class T>
CircularLinkedList<T>* CircularLinkedList<T>::clone() const {
    CircularLinkedList<T>* copy= new CircularLinkedList<T>();
    each([&copy](const T& elem) -> void {
        copy->add(elem);
    });
    return copy;
}


template <class T>
bool CircularLinkedList<T>::equals(initializer_list<T> collection) const {
    if (listSize != collection.size()) {
        return false;
    }

    Node<T> *ptr= tail->next;
    auto it= collection.begin();
    for(it; *it == ptr->value && it != collection.end(); it++, ptr= ptr->next);

    return *it == ptr->value && it == collection.end();
}

template <class T>
bool CircularLinkedList<T>::equals(const Collection<T>* collection) const {
    if (listSize != collection->size()) {
        return false;
    }

    bool equal= true;
    Node<T> *ptr= tail->next;
    collection->each([&equal, &ptr](const T& elem) -> void {
        equal= equal && (ptr->value == elem);
        ptr= ptr->next;
    });
    
    return equal;
}

template <class T>
int CircularLinkedList<T>::size() const {
    return listSize;
}

template <class T>
int CircularLinkedList<T>::capacity() const {
    return listSize;
}

template <class T>
bool CircularLinkedList<T>::isEmpty() const {
    return tail == NULL;
}

template <class T>
bool CircularLinkedList<T>::contains(const T& elem) const {
    bool contain= false;

    each([&contain,&elem](const T& myElem) -> void {
        contain= contain || elem == myElem;
    });
    return contain;
}

template <class T>
string CircularLinkedList<T>::toString() const {
    stringstream str;
    bool first(true);

    str << "[";
    each([&str, &first](const T& elem) -> void {
        if (!first) {
            str << ", ";
        }
        str << elem;
        first= false;
    });
    str << "]";
    return str.str();
}

template <class T>
void CircularLinkedList<T>::each(const function<void (const T&)>& lambda) const {
    Node<T> *ptr= tail->next;

    do {
        lambda(ptr->value);
        ptr= ptr->next;
    } while(ptr != tail->next);
}

template <class T>
void CircularLinkedList<T>::each(const function<void (T&)>& lambda) {
    Node<T> *ptr= tail->next;

    do {
        lambda(ptr->value);
        ptr= ptr->next;
    } while(ptr != tail->next);
}

template <class T>
bool CircularLinkedList<T>::remove(const T& elem) {
    Node<T> *ptr(tail->next), *prev(tail);

    do {
        prev= ptr;
        ptr= ptr->next;
    } while(ptr != tail->next && !(ptr->value == elem));

    if (ptr->value == elem) {
        if (ptr == tail) {
            tail= prev;
        }
        prev->next= ptr->next;
        delete ptr;
        return true;
    }
    return false;
}

template <class T>
void CircularLinkedList<T>::add(const T& elem) {
    Node<T> *ptr(new Node<T>());
    ptr->value= elem;

    if (tail == NULL) {
        tail= ptr;
        tail->next= tail;
    } else {
        ptr->next= tail->next;
        tail->next= ptr;
        tail= ptr;
    }
    listSize++;
}

template <class T>
void CircularLinkedList<T>::clear() {
    Node<T> *ptr;

    while((ptr= tail->next) != tail) {
        tail->next= ptr->next;
        delete ptr;
    }

    delete tail;    
    tail= NULL;
    listSize= 0;
}

template <class T>
void CircularLinkedList<T>::resize(int newSize) {
    if (newSize < listSize) {
        Node<T> *ptr(tail->next);

        for(int i= 0; i < newSize; i++) {
            ptr= ptr->next;
        }
        
        Node<T> *it(ptr->next);
        while(it != tail) {
            ptr->next= it->next;
            delete it;
            it= ptr->next;
        }
        tail= ptr;
        tail->next= it->next;
        delete it;
        
        listSize= newSize;
    }
}


template <class T>
void CircularLinkedList<T>::add(int index, const T& elem) {
    Node<T> *ptr(tail->next), *prev(tail);

    if (index >= listSize) {
        T filler;

        if (defaultValue != NULL) {
            filler= *defaultValue;
        }
        for(int i= listSize; i < index; i++) {
            add(filler);
        }
        add(elem);
    } else {
        for(int i= 0; i < index; i++,prev=ptr,ptr= ptr->next);
    
        Node<T> *newNode(new Node<T>());
        newNode->value= elem;
        newNode->next= ptr;
        prev->next= newNode;
    }
}

template <class T>
void CircularLinkedList<T>::set(int index, const T& elem) throw(out_of_range) {
    RANGE_CHECK(index)

    Node<T> *ptr(tail->next);
    for(int i= 0; i < index; i++,ptr= ptr->next);
    ptr->value= elem;
}

template <class T>
T CircularLinkedList<T>::minus(int index) throw(out_of_range) {
    RANGE_CHECK(index)

    Node<T> *ptr(tail->next), *prev(tail);
    for(int i= 0; i < index; i++,ptr= tail,tail= tail->next);

    T value= ptr->value;
    prev->next= ptr->next;
    delete ptr;

    return value;
}

template <class T>
T CircularLinkedList<T>::get(int index) const throw(out_of_range) {
    RANGE_CHECK(index)

    Node<T> *ptr(tail->next);
    for(int i= 0; i < index; i++,ptr= ptr->next);
    return ptr->value;
}

template <class T>
shared_ptr<List<T>> CircularLinkedList<T>::subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument) {
    if (startIndex < 0 || startIndex >= listSize || endIndex < 0 || endIndex >= listSize) {
        stringstream msg;
        msg << "Indices (" << startIndex << ", " << endIndex << ") lay outside the range [0, " << listSize - 1 << "]";
        throw out_of_range(msg.str());
    } else if (endIndex < startIndex) {
        stringstream msg;
        msg << "End index < start index (" << endIndex << " < " << startIndex << ")";
        throw invalid_argument(msg.str());
    }

    CircularLinkedList<T> *newList= new CircularLinkedList<T>();

    Node<T> *ptr;
    int i;
    for(i= 0; i < startIndex; i++, ptr= ptr->next);
    for(i; i < endIndex; i++, ptr= ptr->next) {
        newList->add(ptr->value);
    }

    return shared_ptr<List<T>>(newList);
}

}
}
}

#endif
