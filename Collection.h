#ifndef ETSAI_COLLECTIONS_COLLECTION_H
#define ETSAI_COLLECTIONS_COLLECTION_H

namespace etsai {
namespace collections {

/**
 * Pure virtual class defining the properties of a collection
 * @author etsai
 */
template <class T>
class Collections {
public:

    /**
     * Get the number of elements in the collection
     * @return  Number of elements
     */
    virtual int size() const= 0;
    /**
     * Get the max number of elements the collection can hold
     * @return  Max number of elements
     */
    virtual int capacity() const= 0;
    /**
     * Returns true if the collection holds no elements
     * @return True if empty
     */
    virtual bool isEmpty() const= 0;
    /**
     * Returns true if the collection contains the specific element
     * @param   elem    Element to search for
     * @return  True if element is in collection
     */
    virtual bool contains(const T& elem) const= 0;

    /**
     * Removes the first occurance desired element from the collection.  The function returns true if the collection 
     * was changed from the call
     * @param   elem    Element to remove
     * @return  True if collection changed
     */
    virtual bool remove(const T& elem)= 0;
    /**
     * Adds the element to the collection
     * @param   elem    Element to add
     */
    virtual void add(const T& elem)= 0;
    /**
     * Removes all elements in the collection
     */
    virtual void clear()= 0;
    /**
     * Changes the capacity of the collection to the new size.  This function only alters the capacity of the collection, 
     * not the number of elements stored.  If newSize < current size, data may be lost.
     */
    virtual void resize(int newSize)= 0;
};  //class Collections

}   //namespace collections
}   //namespace etsai

#endif
