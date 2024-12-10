COMPILER=g++
VERSION=c++17
FLAGS= -Wall -Werror
STOPERR= -Wfatal-errors
INCLUDE_FLAG= -I ./ogdf/include
INCLUDE_LIB = -L./ogdf
TARGET=wagwan.out
MEM_TARGET=wagwan_mem.out
ALGORITHM=$(wildcard algorithm/*.cpp)
SRC=./src
SRCS=$(wildcard $(SRC)/*.cpp)



all:
	$(COMPILER) $(SRCS) $(ALGORITHM) $(INCLUDE_FLAG) $(INCLUDE_LIB) --std=$(VERSION) -lOGDF -lCOIN -o $(TARGET)
sanitize:
	$(COMPILER) -g -fsanitize=address $(SRCS) $(ALGORITHM) $(INCLUDE_FLAG) $(INCLUDE_LIB) --std=$(VERSION) -lOGDF -lCOIN -o $(MEM_TARGET)


tight:
	$(COMPILER) $(SRCS) $(FLAGS) $(INCLUDE_FLAG) $(INCLUDE_LIB) $(STOPERR) $(ALGORITHMS) -lOGDF -lCOIN --std=$(VERSION) -o $(TARGET)

clean:
	rm $(TARGET)
