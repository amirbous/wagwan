COMPILER=g++
VERSION=c++17
FLAGS=-Wall -Wextra
TARGET=wagwan.out
SRC=./src
SRCS=$(wildcard $(SRC)/*.cpp)
all:
	$(COMPILER) $(SRCS) $(FLAGS) -o $(TARGET)

clean:
	rm $(TARGET)
