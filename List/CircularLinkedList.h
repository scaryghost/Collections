#ifndef ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H
#define ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H

#include "List.h"

#include <initializer_list>
#include <memory>
#include <sstream>
#include <stdexcept>
#include <iostream>

namespace etsai {
namespace collections {
namespace list {

using std::initializer_list;
using std::invalid_argument;
using std::out_of_range;
using std::shared_ptr;
using std::stringstream;
using std::unique_ptr;

/**
 * Implements the List interface with a circular linked list
 * @author etsai
 */
template <class T>
class CircularLinkedList : public collections::List<T> {
public:
    /**
     * Default constructor that creates an empty list
     */
    CircularLinkedList();
    /**
     * Creates an empty list, that will fill gaps with the default value during expansions
     * @param   defaultValue    Default value to fill gaps
     */
    CircularLinkedList(const T& defaultValue);
    /**
     * Creates a list with the values in the initializer list
     * @param   collection      Collection of values to fill the list with
     */
    CircularLinkedList(initializer_list<T> collection);
    /**
     * Creates a list with the values in the initializer list, using the default value to fill in gaps 
     * for list expansions
     * @param   collection      Collection of values to fill the list with
     * @param   defaultValue    Default value to fill gaps
     */
    CircularLinkedList(initializer_list<T> collection, const T& defaultValue);
    /**
     * Deletes the allocated memory for each node
     */
    ~CircularLinkedList();
    
    virtual CircularLinkedList* clone() const;
    virtual bool equals(initializer_list<T> collection) const;
    virtual bool equals(const Collection<T>* collection) const;
    virtual int size() const;
    virtual int capacity() const;
    virtual bool isEmpty() const;
    virtual bool contains(const T& elem) const;
    virtual void each(const function<void (const T&)>& lambda) const;
    virtual void each(const function<void (T&)>& lambda);

    virtual bool remove(const T& elem); 
    virtual void add(const T& elem);
    /**
     * This function will delete all memory allocated for the list nodes, resetting the size back to 0
     */
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
        shared_ptr<Node<U>> next;
    };
    
    int listSize;
    shared_ptr<Node<T>> tail;
    unique_ptr<T> defaultValue;
};

template <class T>
CircularLinkedList<T>::CircularLinkedList() : listSize(0) {
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(const T& defaultValue) : CircularLinkedList() {
    this->defaultValue.reset(new T(defaultValue));
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(initializer_list<T> collection) : CircularLinkedList() {
    for(auto &elem: collection) {
        add(elem);
    }
}

template <class T>
CircularLinkedList<T>::CircularLinkedList(initializer_list<T> collection, const T& defaultValue) : CircularLinkedList(collection) {
    this->defaultValue.reset(new T(defaultValue));
}

template <class T>
CircularLinkedList<T>::~CircularLinkedList() {
    clear();
    defaultValue.reset();
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
    if (tail == NULL) {
        return true;
    }

    shared_ptr<Node<T>> ptr= tail->next;
    auto it= collection.begin();
    for(it; *it == ptr->value && it != collection.end(); it++, ptr= ptr->next);

    return it == collection.end();
}

template <class T>
bool CircularLinkedList<T>::equals(const Collection<T>* collection) const {
    if (listSize != collection->size()) {
        return false;
    }
    if (tail == NULL) {
        return true;
    }

    bool equal= true;
    shared_ptr<Node<T>> ptr= tail->next;
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
void CircularLinkedList<T>::each(const function<void (const T&)>& lambda) const {
    if (tail != NULL) {
        shared_ptr<Node<T>> ptr= tail->next;

        do {
            lambda(ptr->value);
            ptr= ptr->next;
        } while(ptr != tail->next);
    }
}

template <class T>
void CircularLinkedList<T>::each(const function<void (T&)>& lambda) {
    if (tail != NULL) {
        shared_ptr<Node<T>> ptr= tail->next;

        do {
            lambda(ptr->value);
            ptr= ptr->next;
        } while(ptr != tail->next);
    }
}

template <class T>
bool CircularLinkedList<T>::remove(const T& elem) {
    if (tail != NULL) {
        shared_ptr<Node<T>> ptr(tail->next), prev(tail);

        do {
            prev= ptr;
            ptr= ptr->next;
        } while(ptr != tail->next && !(ptr->value == elem));

        if (ptr->value == elem) {
            if (ptr == tail) {
                tail= prev;
            }
            prev->next= ptr->next;
            listSize--;
            ptr.reset();
            return true;
        }
    }
    return false;
}

template <class T>
void CircularLinkedList<T>::add(const T& elem) {
    shared_ptr<Node<T>> ptr(new Node<T>());
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
    shared_ptr<Node<T>> ptr;

    if (tail != NULL) {
        while((ptr= tail->next) != tail) {
            tail->next= ptr->next;
            ptr.reset();
        }
        tail.reset();
    }
    listSize= 0;
}

template <class T>
void CircularLinkedList<T>::resize(int newSize) {
    if (newSize < listSize) {
        shared_ptr<Node<T>> ptr(tail->next);

        for(int i= 0; i < newSize; i++) {
            ptr= ptr->next;
        }
        
        shared_ptr<Node<T>> it(ptr->next);
        while(it != tail) {
            ptr->next= it->next;
            it.reset();
            it= ptr->next;
        }
        tail= ptr;
        tail->next= it->next;
        it.reset();
        
        listSize= newSize;
    }
}


template <class T>
void CircularLinkedList<T>::add(int index, const T& elem) {
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
        shared_ptr<Node<T>> ptr(tail->next), prev(tail);

        for(int i= 0; i < index; i++,prev=ptr,ptr= ptr->next);
    
        shared_ptr<Node<T>> newNode(new Node<T>());
        newNode->value= elem;
        newNode->next= ptr;
        prev->next= newNode;
        listSize++;
    }
}

template <class T>
void CircularLinkedList<T>::set(int index, const T& elem) throw(out_of_range) {
    this->rangeCheck(index, listSize);

    shared_ptr<Node<T>> ptr(tail->next);
    for(int i= 0; i < index; i++,ptr= ptr->next);
    ptr->value= elem;
}

template <class T>
T CircularLinkedList<T>::minus(int index) throw(out_of_range) {
    this->rangeCheck(index, listSize);

    shared_ptr<Node<T>> ptr(tail->next), prev(tail);
    for(int i= 0; i < index; i++,ptr= tail,tail= tail->next);

    T value= ptr->value;
    prev->next= ptr->next;
    ptr.reset();

    return value;
}

template <class T>
T CircularLinkedList<T>::get(int index) const throw(out_of_range) {
    this->rangeCheck(index, listSize);

    shared_ptr<Node<T>> ptr(tail->next);
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

    shared_ptr<Node<T>> ptr(tail->next);
    int i;
    for(i= 0; i < startIndex; i++, ptr= ptr->next);
    for(i; i <= endIndex; i++, ptr= ptr->next) {
        newList->add(ptr->value);
    }

    return shared_ptr<List<T>>(newList);
}

}
}
}

#endif
