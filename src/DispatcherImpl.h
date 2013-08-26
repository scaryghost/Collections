#ifndef ETSAI_COLLECTIONS_DISPATCHER_IMPL_H
#define ETSAI_COLLECTIONS_DISPATCHER_IMPL_H

#include <typeindex>
#include <typeinfo>
#include "Dispatcher.h"
#include "classes.h"

namespace etsai {
namespace collections {

using std::type_index;
using std::type_info;
using namespace list;

template <class T, class U>
class DispatcherImpl : public Dispatcher<T,U> {
public:
    virtual Collection<U>* create(const Collection<T>* list);
};

template <class T, class U>
Collection<U>* DispatcherImpl<T,U>::create(const Collection<T>* list) {
    type_index typeInfo(typeid(*list));
    
    if (typeInfo == typeid(ArrayList<T>)) {
        return new ArrayList<U>();
    } else if (typeInfo == typeid(CircularLinkedList<T>)) {
        return new CircularLinkedList<U>();
    }
    return NULL;
}

}
}

#endif
