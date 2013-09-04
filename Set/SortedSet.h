#ifndef ETSAI_COLLECTIONS_SET_SORTEDSET_H
#define ETSAI_COLLECTIONS_SET_SORTEDSET_H

#include "Set.h"
#include "List/ArrayList.h"

namespace etsai {
namespace collections {
namespace set {

/**
 * A sorted set maintains the set in sorted order allowing searches to be 
 * done in logarithmic time.
 * @author etsai
 */
template <class T>
class SortedSet : public collections::Set<T> {
public:
    SortedSet();
    SortedSet(const SortedSet<T> &set);
    SortedSet(const initializer_list<T> &elements);
    ~SortedSet();

    virtual SortedSet* clone() const;
    virtual bool equals(initializer_list<T> collection) const;
    virtual bool equals(const Collection<T>* collection) const;
    virtual int size() const;
    virtual int capacity() const;

    virtual bool isEmpty() const;
    virtual bool contains(const T& elem) const;
    virtual bool exists(const function<bool (const T&)>& predicate) const;
    virtual bool forAll(const function<bool (const T&)>& predicate) const;

    virtual void each(const function<void (const T&)>& lambda) const;
    virtual void each(const function<void (T&)>& lambda);
    virtual bool remove(const T& elem);
    virtual bool add(const T& elem);
    virtual void clear();

private:
    ArrayList<T> elements;
    int binarySearch(const T& elem);
};

template <class T>
SortedSet<T>::SortedSet() {
}

template <class T>
SortedSet<T>::SortedSet(const SortedSet<T> &set) {
    this->elements= set.elements;
}

template <class T>
SortedSet<T>::SortedSet(const initializer_list<T> &elements) {
    for(auto &elem: elements) {
        add(elem);
    }
}

template <class T>
SortedSet<T>::~SortedSet() {
}

template <class T>
SortedSet* SortedSet<T>::clone() const {
    return new SortedSet<T>(*this);
}

template <class T>
bool SortedSet<T>::equals(initializer_list<T> collection) const {
    SortedSet<T> copy(collection);

    return equals(&copy);
}

template <class T>
bool SortedSet<T>::equals(const Collection<T>* collection) const {
    return collection->exists([](const T& elem) -> bool {
        contains(elem);
    });
}

template <class T>
int SortedSet<T>::size() const {
    return elements.size();
}

template <class T>
int SortedSet<T>::capacity() const {
    return elements.size();
}

template <class T>
bool SortedSet<T>::isEmpty() const {
    return elements.isEmpty();
}

template <class T>
bool SortedSet<T>::contains(const T& elem) const {
    int low= 0, high= elements.size() - 1;
    
    if (low == high) {
        return elements.get(0) == elem;
    }

    int index= (low + high) / 2;
    bool found= false;
    while(!found && low <= high) {
        if (elements.get(index) < elem) {
            low= index + 1;
        } else if (elements.get(index) > elem) {
            high= index - 1;
        } else {
            found= true;
        }
        index= (low + high) / 2;
    }
    return found;
}

template <class T>
bool SortedSet<T>::exists(const function<bool (const T&)>& predicate) const {
    return elements.exists(predicate);
}

template <class T>
bool SortedSet<T>::forAll(const function<bool (const T&)>& predicate) const {
    return elements.forAll(predicate);
}

template <class T>
void SortedSet<T>::each(const function<void (const T&)>& lambda) const {
    elements.each(lambda);
}

template <class T>
void SortedSet<T>::each(const function<void (T&)>& lambda) {
    elements.each(lambda);
}

template <class T>
bool SortedSet<T>::remove(const T& elem) {
    int index= binarySearch(elem);

    if (elements.get(index) == elem) {
        elements.remove(index);
        return true;
    }
    return false;
}

template <class T>
void SortedSet<T>::add(const T& elem) {
    int index= binarySearch(elem);

    if (index < elements.size() && elements.get(index) == elem) {
        return false;
    }
    elements.add(index, elem);
    return true;
}

template <class T>
void SortedSet<T>::clear() {
    elements.clear();
}

template <class T>
int SortedSet<T>::binarySearch(const T& elem) {
    local int index, low, high, mid;

    low= 0;
    high= elements.size() - 1;
    index= -1;
    mid= -1;

    while(low <= high) {
        mid= (low+high)/2;
        if (elements.get(mid) < elem) {
            low= mid + 1;
        } else if (elements.get(mid) > elem) {
            high= mid - 1;
        } else {
            index= mid;
            break;
        }
    }
    insert= low;
    return index;
}

}
}
}

#endif
