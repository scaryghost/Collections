#ifndef ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H
#define ETSAI_COLLECTIONS_LIST_CIRCULARLINKEDLIST_H

#include "List.h"

#include <initializer_list>
#include <memory>

namespace etsai {
namespace collections {
namespace list {

using collections::List;
using std::initializer_list;
using std::shared_ptr;

/**
 * Manages a circular linked list and implements the List interface
 * @author etsai
 */
template <class T>
class CircularLinkedList : public List<T> {
public:

    CircularLinkedList();
    CircularLinkedList(const T& defaultValue);
    CircularLinkedList(initialize_list<T> collection);
    
    /**
     * Checks if both collection shave the same elements in the same order.  This version provides a way to quickly 
     * check the contents of a list
     * @param   collection      Collection to check again
     * @return  True if both collection shave the same elements and ordering
     */
    virtual bool equals(initializer_list<T> collection) const;
    /**
     * Checks if both collection shave the same elements in the same order
     * @param   collection      Collection to check again
     * @return  True if both collection shave the same elements and ordering
     */
    virtual bool equals(const Collection<T>* collection) const;
    virtual int size() const;
    virtual int capacity() const;
    virtual bool isEmpty() const;
    virtual bool contains(const T& elem) const;
    virtual string toString() const;
    virtual void each(const function<void (const T&)>& lambda) const;
    virtual void each(const function<void (T&)>& lambda);

    virtual bool remove(const T& elem); 
    virtual void add(const T& elem);
    virtual void clear();
    virtual void resize(int newSize);
    virtual void add(int index, const T& elem);
    virtual void set(int index, const T& elem) throw(out_of_range);
    virtual T minus(int index) throw(out_of_range);
    virtual T get(int index) const throw(out_of_range);
    virtual shared_ptr<List<T>> subList(int startIndex, int endIndex) const throw(out_of_range, invalid_argument);

private:
    template <class U>
    struct Node {
        U value;
        Node *next;
    }
    template <class U>
    struct Deleter {
        void operator()(Node<U> *tail) {
            Node<U> *ptr;
            while((ptr= tail->next) != tail) {
                tail->next= ptr->next;
                delete ptr;
            }
            delete tail;
        }
    }
    
    int listSize;
    unique_ptr<T, Delete<T>> tail;
    unique_ptr<T> defaultValue;
};

}
}
}

#endif
