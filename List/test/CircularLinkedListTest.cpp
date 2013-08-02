
#include <functional>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "Collection.h"
#include "List.h"
#include "List/CircularLinkedList.h"

using etsai::collections::Collection;
using etsai::collections::List;
using etsai::collections::list::CircularLinkedList;
using std::cout;
using std::endl;
using std::function;
using std::invalid_argument;
using std::ostream;
using std::out_of_range;
using std::shared_ptr;
using std::vector;

class Integer {
public:
    Integer() : Integer(0) {
    }
    Integer(const Integer& r) : Integer(*(r.ptr)) {
    }
    Integer(int value) {
        ptr= new int(value);
    }
    ~Integer() {
        delete ptr;
        ptr= NULL;
    }
    int get() const {
        return *ptr;
    }
    Integer& operator= (const Integer& r) {
        if (this != &r) {
            delete ptr;
            ptr= new int(*(r.ptr));
        }
        return *this;
    };

private:
    int* ptr;
};

bool operator ==(const Integer& l, const Integer& r) {
    return l.get() == r.get();
}

ostream& operator <<(ostream& os, const Integer& r) {
    os << r.get();
    return os;
}

typedef function<void (void)> UnitTest;

#define RESULT_HANDLER(result)\
    if (result) {\
        pass++; \
        cout << "Pass" << endl;\
    } else {\
        fail++;\
        cout << "Failed" << endl;\
    }

int main(int argc, char **argv) {
    int pass= 0, fail= 0, index= -1;
    vector<UnitTest> unitTests;

    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5}));
        index++;
        cout << "Test " << index << ": Size Test 1= ";
        RESULT_HANDLER(l->size() == 6);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>(1024));
        index++;
        cout << "Test " << index << ": Size Test 2= ";
        RESULT_HANDLER(l->size() == 0);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5}));
        index++;
        cout << "Test " << index << ": Capacity Test 1= ";
        RESULT_HANDLER(l->capacity() == 6);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Remove first = ";
        l->remove(0);
        RESULT_HANDLER(l->equals({1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
        cout << "Hello World!" << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Remove last = ";
        l->remove(9);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Remove middle = ";
        l->remove(5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Add first = " << endl;
        l->add(0, 0);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        index++;
        cout << "Test " << index << ": Add last = ";
        l->add(l->size(), 9);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Add middle = ";
        l->add(5, 5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>());
        index++;
        cout << "Test " << index << ": Add blanke = ";
        l->add(0);
        l->add(1);
        l->add(2);
        RESULT_HANDLER(l->equals({0, 1, 2}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist beginning = ";
        auto sublist= l->subList(0, 4);
        RESULT_HANDLER(sublist->equals({0, 1, 2, 3, 4}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist end = ";
        auto sublist= l->subList(5, 9);
        RESULT_HANDLER(sublist->equals({5, 6, 7, 8, 9}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist middle = ";
        auto sublist= l->subList(3, 7);
        RESULT_HANDLER(sublist->equals({3, 4, 5, 6, 7}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist invalid 1 = ";

        bool exception= false;
        try {
            auto sublist= l->subList(-1, 7);
        } catch (out_of_range &ex) {
            exception= true;
            cout << "Exception! " << ex.what() << endl;
        }
        RESULT_HANDLER(exception);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist invalid 2 = ";

        bool exception= false;
        try {
            auto sublist= l->subList(3, 10);
        } catch (out_of_range &ex) {
            exception= true;
            cout << "Exception! " << ex.what() << endl;
        }
        RESULT_HANDLER(exception);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Sublist invalid 3 = ";

        bool exception= false;
        try {
            auto sublist= l->subList(7, 3);
        } catch (invalid_argument &ex) {
            exception= true;
            cout << "Exception! " << ex.what() << endl;
        }
        RESULT_HANDLER(exception);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2}, 0));
        index++;
        cout << "Test " << index << ": Expand list 1= ";
        l->add(9, 9);
        RESULT_HANDLER(l->equals({0, 1, 2, 0, 0, 0, 0, 0, 0, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4}, 0));
        index++;
        cout << "Test " << index << ": Resize 1= ";
        l->resize(10);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4}) && l->capacity() == 5);
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Resize 2= ";
        l->resize(5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4}) && l->capacity() == l->size());
        cout << l->capacity() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Set 1= ";
        l->set(5, 10);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 10, 6, 7, 8, 9}) && l->size() == 10);
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Set 2= ";
        bool exception= false;

        try {
            l->set(10, 10);
        } catch (out_of_range& ex) {
            exception= true;
            cout << "Exception! " << ex.what() << endl;
        }
        RESULT_HANDLER(exception);
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Set 3= ";
        bool exception= false;

        try {
            l->set(-1, 10);
        } catch (out_of_range& ex) {
            exception= true;
            cout << "Exception! " << ex.what() << endl;
        }
        RESULT_HANDLER(exception);
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Cloning 1= ";
        Collection<Integer> *copy= l->clone();
        RESULT_HANDLER(l->equals(copy) && l->size() == copy->size() && l->capacity() == copy->capacity());
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        index++;
        cout << "Test " << index << ": Cloning 2= ";
        l->clear();
        Collection<Integer> *copy= l->clone();
        RESULT_HANDLER(l->equals(copy) && l->size() == copy->size() && l->capacity() == copy->capacity());
        cout << "Size: " << copy->size() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, -1));
        index++;
        cout << "Test " << index << ": Clear= ";
        l->clear();
        RESULT_HANDLER(l->isEmpty());
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, -1));
        index++;
        cout << "Test " << index << ": Clear then Add 1= ";
        l->clear();
        l->add(10, 1);
        RESULT_HANDLER(l->equals({-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 1}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, -1));
        index++;
        cout << "Test " << index << ": Clear then Add 2= ";
        l->clear();
        l->add(5, 1);
        RESULT_HANDLER(l->equals({-1, -1, -1, -1, -1, 1}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, -1));
        index++;
        cout << "Test " << index << ": Reverse 1= ";
        l->reverse(true);
        RESULT_HANDLER(l->equals({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}, -1));
        index++;
        cout << "Test " << index << ": Reverse 2= ";
        List<Integer> *copy= l->reverse();
        RESULT_HANDLER(copy->equals({9, 8, 7, 6, 5, 4, 3, 2, 1, 0}));
        cout << copy->toString() << endl;
        delete copy;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1}, -1));
        index++;
        cout << "Test " << index << ": Reverse 3= ";
        l->reverse(true);
        RESULT_HANDLER(l->equals({1, 0}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0, 1}, -1));
        index++;
        cout << "Test " << index << ": Reverse 4= ";
        List<Integer> *copy= l->reverse();
        RESULT_HANDLER(copy->equals({1, 0}));
        cout << copy->toString() << endl;
        delete copy;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0}, -1));
        index++;
        cout << "Test " << index << ": Reverse 5= ";
        l->reverse(true);
        RESULT_HANDLER(l->equals({0}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>({0}, -1));
        index++;
        cout << "Test " << index << ": Reverse 6= ";
        List<Integer> *copy= l->reverse();
        RESULT_HANDLER(copy->equals({0}));
        cout << copy->toString() << endl;
        delete copy;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>());
        index++;
        cout << "Test " << index << ": Reverse 7= ";
        l->reverse(true);
        RESULT_HANDLER(l->equals({}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<List<Integer>> l(new CircularLinkedList<Integer>());
        index++;
        cout << "Test " << index << ": Reverse 8= ";
        List<Integer> *copy= l->reverse();
        RESULT_HANDLER(copy->equals({}));
        cout << copy->toString() << endl;
        delete copy;
    });


    for(UnitTest& test: unitTests) {
        test();
    }
    cout << "Final result: Pass= " << pass << "\tFail=" << fail << endl;
    return 0;
}

