#ifndef IO_HPP
#define IO_HPP


#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace ogdf;


void initializeGraphFromJson(Graph &G, GraphAttributes &GA, const std::string &filePath, std::unordered_map<node, int>& nodesId, int& width, int& height);

void writeGraphToJson(Graph &G, GraphAttributes &GA, const std::string &filePath, std::unordered_map<node, int>& nodesId, int height, int width);

void writeGraphToWagwan(Graph &G, GraphAttributes &GA, const std::string &filePath, std::unordered_map<node, int>& nodesId, int height, int width, ogdf::edge);


#endif
