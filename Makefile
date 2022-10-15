.PHONY: clean

CC=g++
FLAGS=-Wall -Wextra -Werror 	
BUILD=build/
SOURCEDIR=src
HEADFILES=$(wildcard $(SOURCEDIR)/*.hpp)
SOURCES=$(wildcard $(SOURCEDIR)/*.cpp)

main: $(SOURCEDIR)/main.o
	$(CC) $(FLAGS) -o $(BUILD)program $(SOURCEDIR)/main.o 

main.o: $(SOURCEDIR)/Deque.hpp
	$(CC) $(FLAGS) -c $(SOURCES)$


clean:
	rm -Rfv $(SOURCEDIR)/*.o $(SOURCEDIR)/*.gch $(SOURCEDIR)/*.out $(BUILD)program
