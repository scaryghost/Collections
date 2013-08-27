#ifndef ETSAI_COLLECTIONS_DISPATCHER_H
#define ETSAI_COLLECTIONS_DISPATCHER_H

namespace etsai {
namespace collections {

template <class Y>
class Collection;

/**
 * Pure virtual class that acts as a 3rd party class which knows all 
 * derived classes of the Collection abstract class.  The class' goal 
 * is to provide a way to create a new collection of type U, with the 
 * caveat that the collection is the same derived class of the input 
 * collection of type T.
 * @author etsai
 */
template <class T, class U>
class Dispatcher {
public:
    /**
     * Creates an empty collection of type U that is the same type as the 
     * input's derived collection class
     * @param   collection  The collection whose class to copy
     * @return  Empty collection containing type U, that is the same type 
     * as the input
     */
    virtual Collection<U>* create(const Collection<T>* collection)= 0;
    
};

}
}

#endif
