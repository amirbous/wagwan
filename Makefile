COMPILER=g++
VERSION=c++17
FLAGS= -Wfatal-errors -Werror -Wall 
INCLUDE_FLAG=-I include
ALGORITHMS=$(wildcard algorithms/*.cpp)
TARGET=wagwan.out
SRC=./src
SRCS=$(wildcard $(SRC)/*.cpp)
all:
	$(COMPILER) $(SRCS) $(FLAGS) $(INCLUDE_FLAG) $(ALGORITHMS)  --std=$(VERSION) -o $(TARGET)

clean:
	rm $(TARGET)
