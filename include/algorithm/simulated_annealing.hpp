
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node,int> &nodes_id, int iterations, int initial_area);

#endif //SIMULATED_ANNEALING_H
