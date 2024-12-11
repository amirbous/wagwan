#ifndef INTERSECTGRAPH_HPP
#define INTERSECTGRAPH_HPP

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


bool edgesIntersect(const ogdf::GraphAttributes& GA, ogdf::edge& e1, ogdf::edge& e2);



#endif