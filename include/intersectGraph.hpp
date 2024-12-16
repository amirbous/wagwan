#ifndef INTERSECTGRAPH_HPP
#define INTERSECTGRAPH_HPP

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


bool isSharedNode(ogdf::node n1, ogdf::node n2, ogdf::node n3, ogdf::node n4);
int orientation(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c);
bool onSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c);
bool check_node_on_edge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &new_source);
bool edgesIntersect(const ogdf::GraphAttributes& GA, ogdf::edge& e1, ogdf::edge& e2);



#endif