#ifndef ETSAI_COLLECTIONS_LIST_H
#define ETSAI_COLLECTIONS_LIST_H

#include "Collection.h"

#include <sstream>
#include <stdexcept>

namespace etsai {
namespace collections {

using std::out_of_range;
using std::invalid_argument;
using std::stringstream;

/**
 * A list is a collection where specific elements can be accessed via numerical indices.  This interface 
 * defines the functions that map the indices to the elements in the collection
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
     * more space should be allocated to accommodate the index.
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
     * Creates a sublist starting from the start index to the end index.  It is the function caller's responsibility to free 
     * the allocated memory.
     * @param   startIndex  Index to start from
     * @param   endIndex    Index to end at
     * @return  Sublist of the List
     * @throw out_of_range      If either start or end index is outside the range [0, list size - 1]
     * @throw invalid_argument  If endIndex < startIndex
     */
    virtual List<T>* subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument)= 0;
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
