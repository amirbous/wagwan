#include <iostream>
#include <vector>
#include <string>
#include "../include/IO.hpp"

#include "../include/graph.hpp"


// important should be change just made placehoders to reduce compilation warnings
std::vector<Node> readGraph(std::string filename) {
    std::cout << "reading graph from file " << filename << std::endl; 
    std::vector<Node> nodes;
    return nodes;
}


bool writeGraph(std::string filename, Graph graph){

    std::cout << "writing graph to file " << filename << std::endl;  
    graph.getStats();


    return false;
}
