#ifndef REARRANGEGRAPH_HPP
#define REARRANGEGRAPH_HPP

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


void adjustCoordinatesToGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
    std::set<std::pair<int, int>>& populatedPositions, double gridWidth, double gridHeight);
void rearrangeToIntGraph(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
    std::set<std::pair<int, int>> occupiedPositions, double gridWidth, double gridHeight); 
void centerInGrid(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
    double gridWidth, double gridHeight);

void resolveNodeOnEdge(ogdf::Graph &G, ogdf::GraphAttributes &GA,
                        std::set<std::pair<int, int>>& populatedPositions, ogdf::node u,
                        double gridWidth, double gridHeight);

#endif