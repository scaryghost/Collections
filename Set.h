#ifndef ETSAI_COLLECTIONS_SET_H
#define ETSAI_COLLECTIONS_SET_H

#include "Collection.h"

namespace etsai {
namespace collections {

/**
 * A set is defined to be a collection that contains only unique elements.
 * @author etsai
 */
template <class T>
class Set : public Collection<T> {
public:
    virtual ~Set()= 0;
};

template <class T>
Set<T>::~Set() {
}

}
}

#endif
