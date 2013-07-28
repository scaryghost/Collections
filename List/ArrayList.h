#ifndef ETSAI_COLLECTIONS_LIST_ARRAYLIST_H
#define ETSAI_COLLECTIONS_LIST_ARRAYLIST_H

#include <functional>
#include <initializer_list>
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
using std::function;
using std::initializer_list;
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
     * Constructs an empty ArrayList with 0 size and capacity
     */
    ArrayList();
    /**
     * Constructs an ArrayList containing the elements in the initialier list.  This constructor provides a quick way to 
     * create an ArrayList with the elements already known
     * @param   elements    Initial values for the list
     */
    ArrayList(initializer_list<T> elements);
    /**
     * Constructs an ArrayList containing the elements in the initialier list and a reserved default value for list expansions.  
     * This constructor provides a quick way to create an ArrayList with the elements already known
     * @param   elements        Initial values for the list
     * @param   defaultValue    Default value to use for list expansions
     */
    ArrayList(initializer_list<T> elements, const T& defaultValue);
    /**
     * Constructs an ArrayList with an initial capacity.  The array will be able hold the given number of elements but 
     * will have an effective size of 0 until elements are added.
     * @param   initialCapacity     Initial capacity of the list that must be a positive number
     */
    ArrayList(int initialCapacity);
    /**
     * Constructs an ArrayList with an initial size, filled with a default value.  The default value will be used 
     * whenever the list expandes.   The array will be able hold the given number of elements but will have an 
     * effective size of 0 until elements are added
     * @param   initialSize         Initial size of the list
     * @param   defaultValue        Default value to fill the list with
     */
    ArrayList(int initialSize, const T& defaultValue);

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
    unique_ptr<T> defaultValue;
};

template <class T>
ArrayList<T>::ArrayList() : ArrayList(0) {
}

template <class T>
ArrayList<T>::ArrayList(initializer_list<T> elements) : ArrayList(elements.size()) {
    int offset(0);

    for(auto &elem: elements) {
        this->elements.get()[offset]= elem;
        offset++;
    }
    listSize= listCapacity;
}

template <class T>
ArrayList<T>::ArrayList(initializer_list<T> elements, const T& defaultValue) : ArrayList(elements) {
    this->defaultValue.reset(new T(defaultValue));
}

template <class T>
ArrayList<T>::ArrayList(int initialCapacity) : listCapacity(initialCapacity), listSize(0), elements(NULL, ListDeleter<T>()) {
    if (initialCapacity > 0) {
        elements.reset(new T[listCapacity]);
    }
}

template <class T>
ArrayList<T>::ArrayList(int initialSize, const T& defaultValue) : ArrayList(initialSize) {
    this->defaultValue.reset(new T(defaultValue));
    for(int i= 0; i < initialSize; i++) {
        this->elements.get()[i]= defaultValue;
    }
}

template <class T>
bool ArrayList<T>::equals(initializer_list<T> collection) const {
    int index= 0;
    bool equal= true;

    if (collection.size() != listSize) {
        return false;
    }
    for(auto elem: collection) {
        equal= equal && (elements.get()[index] == elem);
        index++;
    }
    return equal;
}

template <class T>
bool ArrayList<T>::equals(const Collection<T>* collection) const {
    int index= 0;
    bool equal= true;

    if (collection->size() != listSize) {
        return false;
    }
    collection->each([&equal, &index, this](const T& elem) -> void {
        equal= equal && (elements.get()[index] == elem);
        index++;
    });
    return equal;
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
    function<void (int)> forAll= [this, &forAll, &found, &elem](int index) -> void {
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
string ArrayList<T>::toString() const {
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
void ArrayList<T>::each(const function<void (const T&)>& lambda) const {
    function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
        if (index >= listSize) {
            return;
        }
        lambda(elements.get()[index]);
        forAll(index + 1);
    };
    forAll(0);
}

template <class T>
void ArrayList<T>::each(const function<void (T&)>& lambda) {
    function<void (int)> forAll= [this, &forAll, &lambda](int index) -> void {
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
    function<void (int)> forAll= [this, &forAll, &elemIndex, &elem](int index) -> void {
        if (index >= listSize) {
            return;
        }
        if (elements.get()[index] == elem) {
            elemIndex= index;
            return;
        }
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
    if (newSize > 0 && newSize != listCapacity) {
        int offset= newSize - listCapacity;

        T *newList= new T[newSize];
        if (listCapacity > 0) {
            int maxLen= (offset < 0 ? newSize : listCapacity);
            for(int i= 0; i < maxLen; i++) {
                newList[i]= elements.get()[i];
            }
        }
        if (offset > 0) {
            if (defaultValue != NULL) {
                for(int i= 0; i < offset; i++) {
                    newList[listCapacity + i]= *defaultValue;
                }
            }
        } else {
            listSize= newSize;
        }
        elements.reset(newList);
        listCapacity= newSize;
    }
}

template <class T>
void ArrayList<T>::add(int index, const T& elem) {
    if (elements == NULL) {
        resize(8);
    } else if (index > listCapacity) {
        resize((index + 1) * 1.5);
    }
    if (index > listSize) {
        listSize= index + 1;
    } else {
        if (listSize + 1 > listCapacity) {
            resize(listCapacity * 1.5);
        }
        for(int i= listSize + 1; i > index; i--) {
            elements.get()[i]= elements.get()[i - 1];
        }
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
    listSize--;
    for(int i= index; i < listSize; i++) {
        elements.get()[i]= elements.get()[i + 1];
    }
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
    for(int i= 0; i < (endIndex - startIndex + 1); i++) {
        newList->elements.get()[i]= elements.get()[startIndex + i];
    }
    newList->listSize= newList->listCapacity;
    return shared_ptr<List<T>>(newList);
}

}
}
}

#endif
