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
    List<int>* list= new ArrayList<int>({0, 1, 2});


    cout << list->toString() << endl;
    cout << "empty? " << list->isEmpty() << endl;

    list->minus(1);
    cout << list->toString() << endl;
    cout << "Size: " << list->size() << endl;

    List<int>* list2= new ArrayList<int>({0, 2});
    cout << "Equals? " << list2->equals(list) << endl;
    return 0;
}
