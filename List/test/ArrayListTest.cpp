#include <iostream>
#include <memory>

#include "List.h"
#include "List/ArrayList.h"

using etsai::collections::List;
using etsai::collections::list::ArrayList;
using std::cout;
using std::endl;
using std::unique_ptr;

int main(int argc, char **argv) {
    int count= 0;
    List<int>* list= new ArrayList<int>(10, 0);


    list->add(0);
    list->add(1);
    list->add(2);
    list->each([](int& i) -> void { cout << i << " "; });
    cout << endl;
    cout << "empty? " << list->isEmpty() << endl;

    list->minus(1);
    list->each([](int& i) -> void { cout << i << " "; });
    cout << endl;
    cout << "Size: " << list->size() << endl;
    return 0;
}
