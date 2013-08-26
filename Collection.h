#ifndef ETSAI_COLLECTIONS_COLLECTION_H
#define ETSAI_COLLECTIONS_COLLECTION_H

#include <functional>
#include <initializer_list>
#include <sstream>
#include <string>
#include <typeinfo>

#include "src/DispatcherImpl.h"

namespace etsai {
namespace collections {

using std::function;
using std::initializer_list;
using std::string;
using std::stringstream;

/**
 * Pure virtual class defining the properties of a collection
 * @author etsai
 */
template <class T>
class Collection {
public:
    /**
     * Pure virtual destructor
     */
    virtual ~Collection()= 0;
    /**
     * Creates a deep copy of the collection
     * @return  Copy of the collection
     */
    virtual Collection* clone() const= 0;
    /**
     * Compares the contents of the intializer list with the collection.  This version is for creating  
     * a collection on the fly to check the contents of the object.  Different collection types 
     * may define equality differently.  Double check the equality condition of the collection type 
     * you are using.
     * @param   collection      The collection to compare against
     8 @returns True if the two collections are equal, false otherwise
     */
    virtual bool equals(initializer_list<T> collection) const= 0;
    /**
     * Compares the contents of the two collections.  Different collection types may define equality 
     * differently.  Double check the equality condition of the collection type you are using 
     * @param   collection      The collection to compare against
     8 @returns True if the two collections are equal, false otherwise
     */
    virtual bool equals(const Collection<T>* collection) const= 0;
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
     * Checks if at least one element satisfies the predicate.  Evaluates p(a0) || p(a1) || ... || p(an)
     * @param   predicate   Lambda that maps T -> bool
     * @return  True if at least one element satisfies the predicate
     */
    virtual bool exists(const function<bool (const T&)>& predicate) const= 0;
    /**
     * Checks if all elements satisfies the predicate.  Evaluates p(a0) && p(a1) && ... && p(an)
     * @param   predicate   Lambda that maps T -> bool
     * @return  True if all elements satisfy the predicate
     */
    virtual bool forAll(const function<bool (const T&)>& predicate) const= 0;
    /**
     * Creates a string representation of the collection
     * @return  String representation of the collection
     */
    virtual string toString() const;
    /**
     * Transforms the collection from T collection -> U collection.  Evaluates [f(a0), f(a2), ..., f(an)].  The caller is 
     * responsible for deallocating the memory.
     * @param   transform   Lambda that maps T -> U
     * @return  Pointer to a C style array of the transformed values
     */
    template <class U>
    Collection<U>* map(const function<U (const T&)>& transform) const;
    /**
     * Applies the lambda to each element in the collection.  This version does not allow you to modify the elements.
     * @param   lambda      Lambda function to evaluate each element with
     */
    virtual void each(const function<void (const T&)>& lambda) const= 0;
    /**
     * Applies the lambda to each element in the collection.  This version allows you to modify the elements.
     * @param   lambda      Lambda function to evaluate each element with
     */
    virtual void each(const function<void (T&)>& lambda)= 0;
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
     * Changes the capacity of the collection to the new size.  If new size > current size, then only the capacity will 
     * be modified.  However, if new size < current size, then both capacity and size will shrink and be equal, and data 
     * will be lost
     */
    virtual void resize(int newSize)= 0;
};  //class Collections

template <class T>
Collection<T>::~Collection() {
}

template <class T>
string Collection<T>::toString() const {
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

template <class T> template <class U>
Collection<U>* Collection<T>::map(const function<U (const T&)>& transform) const {
    Dispatcher<T,U> *dispatcher= new DispatcherImpl<T,U>();
    Collection<U>* converted= dispatcher->create(this);

    each([&converted, &transform](const T& elem) -> void {
        converted->add(transform(elem));
    });
    delete dispatcher;

    return converted;
}

}   //namespace collections
}   //namespace etsai

#endif
