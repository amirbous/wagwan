#ifndef LINESWEEPER_HPP
#define LINESWEEPER_HPP


#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include <vector>
#include <set>




std::vector<std::pair<ogdf::edge, ogdf::edge>> findIntersections(const ogdf::Graph &G, const ogdf::GraphAttributes &GA);

std::map<ogdf::edge, int> calculate_singular_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges);


int calculate_specific_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges, const ogdf::edge &specific_edge);


#endif