#ifndef LINESWEEPER_HPP
#define LINESWEEPER_HPP


#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include <vector>
#include <set>




std::vector<std::pair<ogdf::edge, ogdf::edge>> findIntersections(const ogdf::Graph &G, const ogdf::GraphAttributes &GA);

std::vector<std::pair<int, ogdf::edge>> calculate_singular_intersections(const std::vector<std::pair<ogdf::edge,ogdf::edge>> &edges);

#endif