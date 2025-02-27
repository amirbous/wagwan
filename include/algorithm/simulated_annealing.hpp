
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, int max_iterations, 
                            int width, int height, int cooling_technique, double initial_temperature = 0.95, double cooling_rate = 0.95,
                             int bestCount = 0, const std::string &filePath = "");

#endif //SIMULATED_ANNEALING_H
