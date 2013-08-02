CPP_FLAGS=-std=c++0x -I. -g

all:
	g++ $(CPP_FLAGS) -o ArrayListTest List/test/ArrayListTest.cpp
	g++ $(CPP_FLAGS) -o CircularLinkedListTest List/test/CircularLinkedListTest.cpp

clean:
	rm -Rf ArrayListTest CircularLinkedListTest
