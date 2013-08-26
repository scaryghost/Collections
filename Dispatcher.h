#ifndef ETSAI_COLLECTIONS_DISPATCHER_H
#define ETSAI_COLLECTIONS_DISPATCHER_H

namespace etsai {
namespace collections {

template <class Y>
class Collection;

template <class T, class U>
class Dispatcher {
public:
    virtual Collection<U>* create(const Collection<T>* list)= 0;
    
};

}
}

#endif
