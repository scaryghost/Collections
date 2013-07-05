#ifndef ETSAI_COLLECTIONS_LIST_ARRAYLIST_H
#define ETSAI_COLLECTIONS_LIST_ARRAYLIST_H

#include <functional>
#include <memory>
#include <string.h>
#include "List.h"

namespace etsai {
namespace collections {
namespace list {

/**
 * Manages a standard array and implements the List interface
 * @author etsai
 */
template <class T>
class ArrayList : public collections::List<T> {
public:
    /**
     * Constructs an array with an initial capacity
     * @param   listCapacity    Initial capacity of the list
     * @author etsai
     */
    ArrayList(int initialCapacity) : listCapacity(initialCapacity), listSize(0), elements(NULL, ListDeleter<T>()) {
        elements.reset(new T[listCapacity]);
    }

    virtual int size() const {
        return listSize;
    }
    virtual int capacity() const {
        return listCapacity;
    }
    virtual bool isEmpty() const {
        return listSize == 0;
    }
    virtual bool contains(const T& elem) const {
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
    virtual void each(const std::function<void (const T&)>& lambda) const {
        std::function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
            if (index >= listSize) {
                return;
            }
            lambda(elements.get()[index]);
            forAll(index + 1);
        };
        forAll(0);
    }
    virtual void each(const std::function<void (T&)>& lambda) {
        std::function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
            if (index >= listSize) {
                return;
            }
            lambda(elements.get()[index]);
            forAll(index + 1);
        };
        forAll(0);
    }

    virtual bool remove(const T& elem) {
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
    virtual void add(const T& elem) {
        elements.get()[listSize]= elem;
        listSize++;
    }
    virtual void clear() {
        listSize= 0;
    }
    virtual void resize(int newSize) {
        T *newList= new T[newSize];
        memcpy(newList, elements.get(), newSize);
        elements.reset(newList);
        listCapacity= newSize;
    }
    virtual void add(int index, const T& elem) {
        memmove(elements.get() + index + 1, elements.get() + index, listSize - index);
        elements.get()[index]= elem;
        listSize++;
    }
    virtual void set(int index, const T& elem) {
        elements.get()[index]= elem;
    }
    virtual T minus(int index) {
        T elem= elements.get()[index];
        memmove(elements.get() + index, elements.get() + index + 1, listSize - index);
        listSize--;
        return elem;
    }
    virtual T get(int index) const {
        return elements.get()[index];
    }
    virtual List<T>* subList(int startIndex, int endIndex) const {
        ArrayList<T>* newList= new ArrayList<T>(endIndex - startIndex + 1);

        memcpy(newList->elements.get(), elements.get() + startIndex, endIndex);
        newList->listSize= newList->listCapacity;
        return newList;
    }
private:
    template <class U>
    struct ListDeleter {
        void operator()(U* p) {
            delete [] p;
        }
    };

    int listCapacity, listSize;
    std::shared_ptr<T> elements;
};

}
}
}

#endif
