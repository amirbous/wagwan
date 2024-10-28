#include <iostream>

#include "../include/graph.hpp"
#include "../include/IO.hpp"
#include "../include/utils.hpp"

int main() {


	std::cout << "Welcome to Wagwan" << std::endl;
	std::cout <<"This code runs fine" << std::endl;

    Graph example = readGraph("resources/ex3_k0_xr0.json");

    // TODO: verify output of this result manually or by writing some tests
    // TODO: add parameters
    std::cout <<  "Checking intersection between edge 1 and edge 2 of the graph: " 
        << (edgesIntersect(example, example.getEdges()[0], example.getEdges()[1]) ? "True" : "False")
        << std::endl;

    writeGraph("output.json", example);


}
