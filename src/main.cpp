#include <iostream>
#include "../include/graph.hpp"
#include "../include/IO.hpp"

int main() {


	std::cout << "Welcome to Wagwan" << std::endl;
	std::cout <<"This code runs fine" << std::endl;

    Graph example = readGraph("resources/example.json");
    writeGraph("output.json", example);

}
