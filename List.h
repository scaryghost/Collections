#ifndef ETSAI_COLLECTIONS_LIST_H
#define ETSAI_COLLECTIONS_LIST_H

#include "Collection.h"

#include <functional>
#include <initializer_list>
#include <sstream>
#include <stdexcept>

namespace etsai {
namespace collections {

using std::function;
using std::initializer_list;
using std::invalid_argument;
using std::out_of_range;
using std::stringstream;

/**
 * A list is a collection where specific elements can be accessed via numerical indices.  This 
 * abstract class defines the functions that map the indices to the elements in the collection
 * @author etsai
 */
template <class T>
class List : public Collection<T> {
public:
    /**
     * Pure virtual destructor
     */
    virtual ~List()= 0;
    virtual void add(const T& elem)= 0;
    /**
     * Insert an element at the specific position.  If the index is greater than the list size or capacity, 
     * more space will be allocated to accommodate the index.
     * @param   index   Index to insert the element at
     * @param   elem    Element to insert
     */
    virtual void add(int index, const T& elem)= 0;
    /**
     * Checks if both collections have the same elements in the same order.  This version takes in an 
     * initializer list, providing a convenient way to quickly construct a collection.
     * @param   collection      Collection to check against
     * @return  True if both collections have the same elements and ordering
     */
    virtual bool equals(initializer_list<T> collection) const= 0;
    /**
     * Checks if both collections have the same elements in the same order
     * @param   collection      Collection to check against
     * @return  True if both collections have the same elements and ordering
     */
    virtual bool equals(const Collection<T>* collection) const= 0;
    /**
     * Reverses the ordering of the list.  This version does not change the object, but instead returns a copy of the elements in reversed order.  
     * It is the caller's responsiblity to deallocate the created list.
     * @return  Copy of the list, with reversed ordering
     */
    virtual List<T>* reverse() const= 0;
    /**
     * Reverses the ordering of the list.  This version gives the option of applying the change to the calling object, rather than 
     * creating a copy of the list.  It is the caller's responsiblity to free the allocated memory if mutate is set to false.
     * @param   mutate  Determine if a copy should be created, or apply the change to the calling object
     * @return  if mutate is true, NULL is returned; otherwise, copy of the list with reversed ordering
     */
    virtual List<T>* reverse(bool mutate)= 0;
    /**
     * Replace the element at the specific index with the new element
     * @param   index   Index to replace
     * @param   elem    Element to be stored 
     * throws   out_of_range    If index lies outside the range [0, list size - 1]
     */
    virtual void set(int index, const T& elem) throw(out_of_range)= 0;
    /**
     * Remove the element at the specific index and return the stored value
     * @param   index   Index to remove
     * @return  Element stored at the index
     * @throws  out_of_range    If index lies outside the range [0, list size - 1]
     */
    virtual T minus(int index) throw(out_of_range)= 0;
    /**
     * Get the element stored at the index
     * @param   index   Index to lookup
     * @return  Element at the given index
     * @throws  out_of_range    If index lies outside the range [0, list size - 1]
     */
    virtual T get(int index) const throw(out_of_range)= 0;
    /**
     * Creates a sublist starting from the start index to the end index.  It is the function caller's responsibility to  
     * deallocate the created list
     * @param   startIndex  Index to start from
     * @param   endIndex    Index to end at
     * @return  Sublist of the List
     * @throw out_of_range      If either start or end index is outside the range [0, list size - 1]
     * @throw invalid_argument  If endIndex < startIndex
     */
    virtual List<T>* subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument)= 0;
    /**
     * Applies a function across all elements with an initial value with a left fold ordering.  Evaluates f(...f(f(a, b0), b1), bn)
     * @param   initialValue    Initial value to give to the function
     * @param   lambda          Lambda that takes 2 parameters, mapping (U, T) -> U
     */
    template <class U>
    U foldLeft(const U& initialValue, const function<U (const U&, const T&)>& lambda) const;
    /**
     * Applies a function across all elements with an initial value with a right fold odering.  Evaluates f(b0, ...f(bn-1, f(bn, a)))
     * @param   initialValue    Initial value to give to the function
     * @param   lambda          Lambda that takes 2 parameters, mapping (T, U) -> U
     */
    template <class U>
    U foldRight(const U& initialValue, const function<U (const T&, const U&)>& lambda) const;

protected:
    /**
     * Convenience method to check if the index is between the range [0, size - 1].  The method will throw an 
     * out_of_range excepetion if the index is outside the prescribed boundary.
     * @param   index       Index value to check
     * @param   listSize    Size of the list
     */
    inline void rangeCheck(int index, int listSize) const;
};

template <class T>
List<T>::~List() {
}

template <class T> template <class U>
U List<T>::foldLeft(const U& initialValue, const function<U (const U&, const T&)>& lambda) const {
    U accum;
    int listSize= this->size();
    for(int i= 0; i < listSize; i++) {
        if (i == 0) {
            accum= lambda(initialValue, get(i));
        } else {
            accum= lambda(accum, get(i));
        }
    }

    return accum;
}

template <class T> template <class U>
U List<T>::foldRight(const U& initialValue, const function<U (const T&, const U&)>& lambda) const {
    U accum;
    int end= this->size() - 1;
    for(int i= end; i >= 0; i--) {
        if (i == end) {
            accum= lambda(get(i), initialValue);
        } else {
            accum= lambda(get(i), accum);
        }
    }

    return accum;
}


template <class T>
void List<T>::rangeCheck(int index, int listSize) const {
    if (index < 0 || index >= listSize) {
        stringstream msg;
        msg << "Index (" << index << ") out of range [0, " << listSize - 1 << "]";
        throw out_of_range(msg.str());
    }
}

}   //namespace collections
}   //namespace etsai

#endif
