#ifndef IO_HPP
#define IO_HPP

#include <vector>
#include <string>
#include "graph.hpp"

class Node;
class Edge;
class Graph;


// parse a file and return set of nodes and edges
Graph readGraph(std::string filename);

// takes a graph and prints it to a file
bool writeGraph(std::string filename, Graph graph);

#endif