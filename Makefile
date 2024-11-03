COMPILER=g++
VERSION=c++17
FLAGS= -Wfatal-errors -Werror -Wall 
TARGET=wagwan.out
SRC=./src
SRCS=$(wildcard $(SRC)/*.cpp)
all:
	$(COMPILER) $(SRCS) $(FLAGS) -o $(TARGET) --std=$(VERSION)

clean:
	rm $(TARGET)
