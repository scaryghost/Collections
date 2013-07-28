#include <functional>
#include <iostream>
#include <memory>
#include <vector>

#include "List.h"
#include "List/ArrayList.h"

using etsai::collections::List;
using etsai::collections::list::ArrayList;
using std::cout;
using std::endl;
using std::function;
using std::ostream;
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
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5});
        index++;
        cout << "Test " << index << ": Size Test 1= ";
        RESULT_HANDLER(l->size() == 6);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>(1024);
        index++;
        cout << "Test " << index << ": Size Test 2= ";
        RESULT_HANDLER(l->size() == 0);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>(2048, -1);
        index++;
        cout << "Test " << index << ": Size Test 3 = ";
        RESULT_HANDLER(l->size() == 0);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5});
        index++;
        cout << "Test " << index << ": Capacity Test 1= ";
        RESULT_HANDLER(l->capacity() == 6);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>(1024);
        index++;
        cout << "Test " << index << ": Capacity Test 2= ";
        RESULT_HANDLER(l->capacity() == 1024);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>(2048, -1);
        index++;
        cout << "Test " << index << ": Capacity Test 3 = ";
        RESULT_HANDLER(l->capacity() == 2048);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Remove first = ";
        l->remove(0);
        RESULT_HANDLER(l->equals({1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
        cout << "Hello World!" << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Remove last = ";
        l->remove(9);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Remove middle = ";
        l->remove(5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Add first = " << endl;
        l->add(0, 0);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8});
        index++;
        cout << "Test " << index << ": Add last = ";
        l->add(l->size(), 9);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Add middle = ";
        l->add(5, 5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>();
        index++;
        cout << "Test " << index << ": Add blanke = ";
        l->add(0);
        l->add(1);
        l->add(2);
        RESULT_HANDLER(l->equals({0, 1, 2}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Sublist beginning = ";
        auto sublist= l->subList(0, 4);
        RESULT_HANDLER(sublist->equals({0, 1, 2, 3, 4}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Sublist end = ";
        auto sublist= l->subList(5, 9);
        RESULT_HANDLER(sublist->equals({5, 6, 7, 8, 9}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Sublist middle = ";
        auto sublist= l->subList(3, 7);
        RESULT_HANDLER(sublist->equals({3, 4, 5, 6, 7}));
        cout << sublist->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2}, 0);
        index++;
        cout << "Test " << index << ": Expand list 1= ";
        l->add(9, 9);
        RESULT_HANDLER(l->equals({0, 1, 2, 0, 0, 0, 0, 0, 0, 9}));
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>(10, 0);
        index++;
        cout << "Test " << index << ": Expand list 2= ";
        l->add(4, 5);
        l->add(2, 2);
        RESULT_HANDLER(l->equals({0, 0, 2, 0, 0, 5}));
        cout << l->size() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4}, 0);
        index++;
        cout << "Test " << index << ": Resize 1= ";
        l->resize(10);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4}) && l->capacity() == 10);
        cout << l->toString() << endl;
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        List<Integer> *l= new ArrayList<Integer>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9});
        index++;
        cout << "Test " << index << ": Resize 2= ";
        l->resize(5);
        RESULT_HANDLER(l->equals({0, 1, 2, 3, 4}) && l->capacity() == l->size());
        cout << l->capacity() << endl;
    });

    for(UnitTest& test: unitTests) {
        test();
    }
    cout << "Final result: Pass= " << pass << "\tFail=" << fail << endl;
    return 0;
}

