#ifndef ETSAI_COLLECTIONS_LIST_H
#define ETSAI_COLLECTIONS_LIST_H

namespace etsai {
namespace collections {

/**
 * Pure virtual class defining the properties of a List
 * @author etsai
 */
template <class T>
class List : public Collection<T> {
public:
    /**
     * Insert an element at the specific position
     * @param   index   Index to insert the element at
     * @param   elem    Element to insert
     */
    virtual void add(int index, const T& elem)= 0;
    /**
     * Replace the element at the specific index with the new element
     * @param   index   Index to replace
     * @param   elem    Element to be stored 
     */
    virtual void set(int index, const T& elem)= 0;
    /**
     * Remove the element at the specific index and return the stored value
     * @param   index   Index to remove
     * @return  Element stored at the index
     */
    virtual T remove(int index)= 0;

    /**
     * Get the element stored at the index
     * @param   index   Index to lookup
     * @return  Element at the given index
     */
    virtual T get(int index) const= 0;
    /**
     * Creates a sublist starting from the start index to the end index
     * @param   startIndex  Index to start from
     * @param   endIndex    Index to end at
     * @return  Sublist of the List
     */
    virtual List<T>* subList(int startIndex, int endIndex) const= 0;
};

}   //namespace collections
}   //namespace etsai

#endif
