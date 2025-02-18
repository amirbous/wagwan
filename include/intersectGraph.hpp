#ifndef INTERSECTGRAPH_HPP
#define INTERSECTGRAPH_HPP

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>


std::pair<int, ogdf::edge> getODGFIntersect(const ogdf::Graph &G, const ogdf::GraphAttributes &GA);
int getODGFMax(const ogdf::Graph &G, const ogdf::GraphAttributes &GA);
bool isSharedNode(ogdf::node n1, ogdf::node n2, ogdf::node n3, ogdf::node n4);
int orientation(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c);
bool onSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, ogdf::node c);
bool check_pos_on_anyEdge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const std::pair<int, int>& pos);
bool edgesIntersect(const ogdf::GraphAttributes& GA, ogdf::edge& e1, ogdf::edge& e2);
bool check_node_on_anyEdge(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &new_source);
bool edgeIntersects_anyNode(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::edge &new_edge);
bool positionOnSegment(const ogdf::GraphAttributes& GA, ogdf::node a, ogdf::node b, const std::pair<int, int>& pos);
std::vector<ogdf::edge> getIncidentEdges(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node &u);
bool nodeEdges_intersect_anyNode(const ogdf::Graph &G, const ogdf::GraphAttributes &GA, const ogdf::node & node);
#endif