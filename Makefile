CPP_FLAGS=-std=c++0x -I. -g

all: ArrayListTest CircularLinkedListTest

ArrayListTest: List/test/ArrayListTest.cpp List/ArrayList.h
	g++ $(CPP_FLAGS) -o $@ $<

CircularLinkedListTest: List/test/CircularLinkedListTest.cpp List/CircularLinkedList.h
	g++ $(CPP_FLAGS) -o $@ $<

clean:
	rm -Rf ArrayListTest CircularLinkedListTest
