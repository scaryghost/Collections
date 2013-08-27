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
 * Implements the List abstract with a circular linked list.  For a circular linked list, the size will 
 * always equal the capacity
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
     * Copy constructor
     */
    CircularLinkedList(const CircularLinkedList<T>& list);
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
    virtual bool exists(const function<bool (const T&)>& lambda) const;
    virtual bool forAll(const function<bool (const T&)>& lambda) const;

    virtual bool remove(const T& elem); 
    virtual void add(const T& elem);
    /**
     * This function will delete all memory allocated for the list nodes, resetting the size and capacity back to 0
     */
    virtual void clear();
    virtual CircularLinkedList<T>* reverse() const;
    virtual CircularLinkedList<T>* reverse(bool mutate);
    virtual void resize(int newSize);
    virtual void add(int index, const T& elem);
    virtual void set(int index, const T& elem) throw(out_of_range);
    virtual T minus(int index) throw(out_of_range);
    virtual T get(int index) const throw(out_of_range);
    virtual CircularLinkedList<T>* subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument);

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
CircularLinkedList<T>::CircularLinkedList(const CircularLinkedList<T>& list) {
    listSize= 0;
    list.each([this](const T& elem) -> void {
        this->add(elem);
    });
    if (list.defaultValue != NULL) {
        defaultValue.reset(new T(*(list.defaultValue)));
    }
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
    return new CircularLinkedList<T>(*this);
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
bool CircularLinkedList<T>::exists(const function<bool (const T&)>& lambda) const {
    if (this->isEmpty()) {
        return false;
    }

    bool doesExist= false;
    shared_ptr<Node<T>> ptr(tail->next);

    do {
        doesExist= doesExist || lambda(ptr->value);
        ptr= ptr->next;
    } while(ptr != tail->next && !doesExist);
    return doesExist;
}

template <class T>
bool CircularLinkedList<T>::forAll(const function<bool (const T&)>& lambda) const {
    if (this->isEmpty()) {
        return false;
    }

    bool allTrue= true;
    shared_ptr<Node<T>> ptr(tail->next);

    do {
        allTrue= allTrue && lambda(ptr->value);
        ptr= ptr->next;
    } while(ptr != tail->next && allTrue);
    return allTrue;
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
CircularLinkedList<T>* CircularLinkedList<T>::reverse() const {
    CircularLinkedList<T>* copy= (defaultValue == NULL) ? new CircularLinkedList<T>() : new CircularLinkedList<T>(*defaultValue);

    each([&copy](const T& elem) -> void {
        copy->add(0, elem);
    });
    return copy;
}

template <class T>
CircularLinkedList<T>* CircularLinkedList<T>::reverse(bool mutate) {
    if (!mutate) {
        return reverse();
    }

    if (tail != NULL) {
        shared_ptr<Node<T>>* elements= new shared_ptr<Node<T>>[listSize];
        shared_ptr<Node<T>> ptr= tail->next;
        int index= 0;
    
        do {
            elements[index]= ptr;
            ptr= ptr->next;
            index++;
        } while(ptr != tail->next);
        for(int rIndex= listSize - 1; rIndex > 0; rIndex--) {
            elements[rIndex]->next= elements[rIndex - 1];
        }
        elements[0]->next= elements[listSize - 1];
        tail= elements[0];

        delete []elements;
    }

    return NULL;
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
CircularLinkedList<T>* CircularLinkedList<T>::subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument) {
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

    return newList;
}

}
}
}

#endif
