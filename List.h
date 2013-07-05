#ifndef ETSAI_COLLECTIONS_LIST_H
#define ETSAI_COLLECTIONS_LIST_H

#include <stdexcept>

namespace etsai {
namespace collections {

using std::out_of_range;
using std::invalid_argument;

/**
 * Pure virtual class defining the properties of a List
 * @author etsai
 */
template <class T>
class List : public Collection<T> {
public:
    /**
     * Insert an element at the specific position.  If the index is greater than the list size or capacity, 
     * more space should be allocated to accommodate the index.
     * @param   index   Index to insert the element at
     * @param   elem    Element to insert
     */
    virtual void add(int index, const T& elem)= 0;
    /**
     * Replace the element at the specific index with the new element
     * @param   index   Index to replace
     * @param   elem    Element to be stored 
     * throws   out_of_range    If the index >= list size
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
     * Creates a sublist starting from the start index to the end index
     * @param   startIndex  Index to start from
     * @param   endIndex    Index to end at
     * @return  Sublist of the List
     * @throw out_of_range      If either start or end index is outside the range [0, list size - 1]
     * @throw invalid_argument  If endIndex < startIndex
     */
    virtual List<T>* subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument)= 0;
};

}   //namespace collections
}   //namespace etsai

#endif
