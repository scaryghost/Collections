#ifndef ETSAI_COLLECTIONS_DISPATCHER_H
#define ETSAI_COLLECTIONS_DISPATCHER_H

#include <cstring>
#include <typeinfo>

namespace etsai {
namespace collections {

using std::type_info;
using std::strpbrk;

template <class T>
class Collection;

template <class T>
class List;

namespace list {
template <class T>
class ArrayList;

template <class T>
class CircularLinkedList;
}

class Dispatcher {
public:
    template <class T>
    static Collection<T>* create(const type_info& type);
};

template <class T>
Collection<T>* Dispatcher::create(const type_info& type) {
    const char* typeName= type.name();
    
    if (strpbrk(typeid(list::ArrayList<T>).name(), typeName) != NULL) {
        return new list::ArrayList<T>();
    } else if (strpbrk(typeid(list::CircularLinkedList<T>).name(), typeName) != NULL) {
        return new list::CircularLinkedList<T>();
    }
    return NULL;
}

}
}

#endif
