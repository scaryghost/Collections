#include "Set.h"
#include "Set/SortedSet.h"

#include <iostream>
#include <memory>
#include <vector>

using namespace etsai::collections;
using namespace etsai::collections::set;
using namespace std;

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
        shared_ptr<Set<int>> s(new SortedSet<int>({0, 1, 2, 3, 4, 5}));
        index++;
        cout << "Test " << index << ": Size Test 1= ";
        RESULT_HANDLER(s->size() == 6);
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>());
        index++;
        s->add(5);
        s->add(3);
        s->add(7);
        s->add(6);
        cout << "Test " << index << ": Add Test 1= ";
        RESULT_HANDLER(s->equals({3, 5, 6, 7}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 3, 7}));
        index++;
        s->add(6);
        s->add(2);
        s->add(8);
        cout << "Test " << index << ": Add Test 2= ";
        RESULT_HANDLER(s->equals({2, 3, 5, 6, 7, 8}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Contains Test 1= ";
        RESULT_HANDLER(!s->contains(-1));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Contains Test 2= ";
        RESULT_HANDLER(!s->contains(10));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Contains Test 3= ";
        RESULT_HANDLER(!s->contains(4));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "set: " << s->toString() << endl;
        cout << "Test " << index << ": Contains Test 4= ";
        RESULT_HANDLER(s->contains(5));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "set: " << s->toString() << endl;
        cout << "Test " << index << ": Contains Test 5= ";
        RESULT_HANDLER(s->contains(0));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "set: " << s->toString() << endl;
        cout << "Test " << index << ": Contains Test 6= ";
        RESULT_HANDLER(s->contains(9));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Duplicates 1= ";
        RESULT_HANDLER(!s->add(0));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Duplicates 2= ";
        RESULT_HANDLER(!s->add(9));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Duplicates 3= ";
        RESULT_HANDLER(!s->add(5));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Add 1= ";
        RESULT_HANDLER(s->add(-1) && s->equals({5, 4, 3, 7, 0, 1, 9, 2, 6, 8, -1}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Add 2= ";
        RESULT_HANDLER(s->add(10) && s->equals({5, 4, 3, 7, 0, 1, 9, 2, 6, 8, 10}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Add 3= ";
        RESULT_HANDLER(s->add(5) && s->equals({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Remove 1= ";
        RESULT_HANDLER(s->remove(0) && s->equals({5, 4, 3, 7, 1, 9, 2, 6, 8}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Remove 2= ";
        RESULT_HANDLER(s->remove(9) && s->equals({5, 4, 3, 7, 0, 1, 2, 6, 8}));
    });
    unitTests.push_back([&pass, &fail, &index]() -> void {
        shared_ptr<Set<int>> s(new SortedSet<int>({5, 4, 3, 7, 0, 1, 9, 2, 6, 8}));
        index++;
        cout << "Test " << index << ": Remove 3= ";
        RESULT_HANDLER(s->remove(5) && s->equals({4, 3, 7, 0, 1, 9, 2, 6, 8}));
    });
    for(UnitTest& test: unitTests) {
        test();
    }
    cout << "Final result: Pass= " << pass << "\tFail=" << fail << endl;
    return 0;
}
