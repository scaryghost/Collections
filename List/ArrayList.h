#ifndef ETSAI_COLLECTIONS_LIST_ARRAYLIST_H
#define ETSAI_COLLECTIONS_LIST_ARRAYLIST_H

#include <functional>
#include <memory>
#include <sstream>
#include <string.h>
#include "List.h"

#define RANGE_CHECK(index) \
if (index < 0 || index >= listSize) { \
    stringstream msg; \
    msg << "Index (" << index << ") out of range [0, " << listSize - 1 << "]"; \
    throw out_of_range(msg.str()); \
}

namespace etsai {
namespace collections {
namespace list {

using collections::List;
using std::invalid_argument;
using std::out_of_range;
using std::shared_ptr;
using std::stringstream;
using std::unique_ptr;

/**
 * Manages a standard array and implements the List interface
 * @author etsai
 */
template <class T>
class ArrayList : public List<T> {
public:
    /**
     * Constructs an array with an initial capacity.  The array will be able hold the given number of elements but 
     * will have an effective size of 0 until elements are added.
     * @param   initialCapacity     Initial capacity of the list
     */
    ArrayList(int initialCapacity);
    /**
     * Constructs an array with an initial capacity, filled with a default value.  The default value will be used 
     * whenever the list expandes.   The array will be able hold the given number of elements but will have an 
     * effective size of 0 until elements are added
     * @param   initialCapacity     Initial capacity of the list
     * @param   defaultValue        Default value to fill the list with
     */
    ArrayList(int initialCapacity, const T& defaultValue);

    virtual int size() const;
    virtual int capacity() const;
    virtual bool isEmpty() const;
    virtual bool contains(const T& elem) const;
    virtual void each(const std::function<void (const T&)>& lambda) const;
    virtual void each(const std::function<void (T&)>& lambda);

    virtual bool remove(const T& elem); 
    virtual void add(const T& elem);
    virtual void clear();
    virtual void resize(int newSize);
    virtual void add(int index, const T& elem);
    virtual void set(int index, const T& elem) throw(out_of_range);
    virtual T minus(int index) throw(out_of_range);
    virtual T get(int index) const throw(out_of_range);
    virtual shared_ptr<List<T>> subList(int startIndex, int endIndex) const throw(out_of_range);
private:
    template <class U>
    struct ListDeleter {
        void operator()(U* p) {
            delete [] p;
        }
    };

    int listCapacity, listSize;
    unique_ptr<T, ListDeleter<T> > elements;
    unique_ptr<int> defaultValue;
};

template <class T>
ArrayList<T>::ArrayList(int initialCapacity) : listCapacity(initialCapacity), listSize(0), elements(NULL, ListDeleter<T>()) {
    elements.reset(new T[listCapacity]);
}

template <class T>
ArrayList<T>::ArrayList(int initialCapacity, const T& defaultValue) : ArrayList(initialCapacity) {
    defaultValue.reset(new T(defaultValue));
}

template <class T>
int ArrayList<T>::size() const {
    return listSize;
}

template <class T>
int ArrayList<T>::capacity() const {
    return listCapacity;
}

template <class T>
bool ArrayList<T>::isEmpty() const {
    return listSize == 0;
}

template <class T>
bool ArrayList<T>::contains(const T& elem) const {
    bool found= false;
    std::function<void (int)> forAll= [this, &forAll, &found, &elem](int index) -> void {
        if (index >= listSize || found) {
            return;
        }
        found= elements.get()[index] == elem;
        forAll(index + 1);
    };
    forAll(0);
    return found;
}

template <class T>
void ArrayList<T>::each(const std::function<void (const T&)>& lambda) const {
    std::function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
        if (index >= listSize) {
            return;
        }
        lambda(elements.get()[index]);
        forAll(index + 1);
    };
    forAll(0);
}

template <class T>
void ArrayList<T>::each(const std::function<void (T&)>& lambda) {
    std::function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
        if (index >= listSize) {
            return;
        }
        lambda(elements.get()[index]);
        forAll(index + 1);
    };
    forAll(0);
}

template <class T>
bool ArrayList<T>::remove(const T& elem) {
    int elemIndex= -1;
    std::function<void (int)> forAll= [this, &forAll, &elemIndex, &elem](int index) -> void {
        if (index >= listSize) {
            return;
        }
        elemIndex= elements.get()[index] == elem;
        forAll(index + 1);
    };
    forAll(0);
    if (elemIndex == -1) {
        return false;
    }
    minus(elemIndex);
    return true;
}

template <class T>
void ArrayList<T>::add(const T& elem) {
    add(listSize, elem);
}

template <class T>
void ArrayList<T>::clear() {
    listSize= 0;
}

template <class T>
void ArrayList<T>::resize(int newSize) {
        int offset= newSize - listCapacity;

        T *newList= new T[newSize];
        memcpy(newList, elements.get(), newSize);
        if (defaultValue != NULL) {
            memset(newList + listCapacity, *defaultValue, offset);
        }
        elements.reset(newList);
        listCapacity= newSize;
    }

template <class T>
void ArrayList<T>::add(int index, const T& elem) {
    if (index > listCapacity) {
        resize(listCapacity * 1.5);
    }
    if (index > listSize) {
        listSize= index;
    } else {
        memmove(elements.get() + index + 1, elements.get() + index, listSize - index);
        listSize++;
    }
    elements.get()[index]= elem;
}

template <class T>
void ArrayList<T>::set(int index, const T& elem) throw(out_of_range) {
    RANGE_CHECK(index)
    elements.get()[index]= elem;
}

template <class T>
T ArrayList<T>::minus(int index) throw(out_of_range) {
    RANGE_CHECK(index)
    T elem= elements.get()[index];
    memmove(elements.get() + index, elements.get() + index + 1, listSize - index);
    listSize--;
    return elem;
}

template <class T>
T ArrayList<T>::get(int index) const throw(out_of_range) {
    RANGE_CHECK(index)
    return elements.get()[index];
}

template <class T>
shared_ptr<List<T>> ArrayList<T>::subList(int startIndex, int endIndex) const throw(out_of_range) {
    if (startIndex < 0 || startIndex >= listSize || endIndex < 0 || endIndex >= listSize) {
        stringstream msg;
        msg << "Indices (" << startIndex << ", " << endIndex << ") lay outside the range [0, " << listSize - 1 << "]";
        throw out_of_range(msg.str());
    } else if (endIndex < startIndex) {
        stringstream msg;
        msg << "End index < start index (" << endIndex << " < " << startIndex << ")";
        throw invalid_argument(msg.str());
    }
        
    ArrayList<T>* newList= new ArrayList<T>(endIndex - startIndex + 1);
    memcpy(newList->elements.get(), elements.get() + startIndex, endIndex);
    newList->listSize= newList->listCapacity;
    return shared_ptr<List<T>>(newList);
}

}
}
}

#endif
