COMPILER=g++
VERSION=c++17
FLAGS= -Wall -Werror
STOPERR= -Wfatal-errors
INCLUDE_FLAG= -I ./ogdf/include
INCLUDE_LIB = -L./ogdf
TARGET=wagwan.out
ALGORITHM=$(wildcard algorithm/*.cpp)
SRC=./src
SRCS=$(wildcard $(SRC)/*.cpp)


all:
	$(COMPILER) $(SRCS) $(ALGORITHM) $(INCLUDE_FLAG) $(INCLUDE_LIB) --std=$(VERSION) -lOGDF -lCOIN -o $(TARGET)

tight:
	$(COMPILER) $(SRCS) $(FLAGS) $(INCLUDE_FLAG) $(INCLUDE_LIB) $(STOPERR) $(ALGORITHMS) -lOGDF -lCOIN --std=$(VERSION) -o $(TARGET)

clean:
	rm $(TARGET)
