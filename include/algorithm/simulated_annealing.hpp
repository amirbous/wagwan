
#ifndef SIMULATED_ANNEALING_H
#define SIMULATED_ANNEALING_H

#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../../include/RTree.h"
#include "../../include/structs.hpp"    

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, 
                        int max_iterations, RTree<Rectangle*, double, 2> & rtree, std::map<ogdf::edge, Rectangle*> & edgeRectangle, 
                            int width, int height, int cooling_technique, double initial_temperature, double cooling_rate);
#endif //SIMULATED_ANNEALING_H
