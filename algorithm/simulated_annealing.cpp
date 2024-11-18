#include "../include/algorithm/simulated_annealing.hpp"
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

void simulated_annealing(ogdf::Graph &G, int iterations, int initial_area)
{
    std::map<ogdf::node, bool> check_node;
    for(auto node: G.nodes)
    {
        check_node[node] = true;
    }

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        std::vector<std::pair<int, ogdf::edge>> intersection_edges = calculateIntersections(G);
        for(auto worst_edge: intersection_edges)
        {
            ogdf::edge edge = worst_edge.second;
            ogdf::node source = edge->source();
            ogdf::node target = edge->target();


            int source_x = source.getX();
            int source_y = source.getY();

            G.delEdge(edge);
            ogdf::node best_node;
            int min_intersections = worst_edge.first;
            for (int explore = 1; explore <= initial_area; explore++)
            {
                // explore represents the layers around our source, that go up to the initial area
                int addition = explore+iteration;

                // potential sources, this does not work yet, as it only considers the first ring of positions
                ogdf::node new_source_1(source_x+addition,source_y);
                ogdf::node new_source_2(source_x,source_y+addition);
                ogdf::node new_source_3(source_x+addition,source_y+addition);
                ogdf::node new_source_4(source_x-addition,source_y);
                ogdf::node new_source_5(source_x,source_y-addition);
                ogdf::node new_source_6(source_x-addition,source_y-addition);
                ogdf::node new_source_7(source_x+addition,source_y-addition);
                ogdf::node new_source_8(source_x-addition,source_y+addition);

                // vector with the new potential nodes, for this specific layer and iteration
                std::vector new_sources = {source, new_source_1,new_source_2,new_source_3,new_source_4,new_source_5,new_source_6,new_source_7,new_source_8};

                // now we iterate over them, check if there is no current node there
                // we add the potential edge to a temp graph, calculate the max intersection
                // and update the best_node and min_intersection accordingly
                for(auto new_source: new_sources)
                {
                    if (!check_node[new_source])
                    {
                        ogdf::Graph new_G = G;
                        new_G.newEdge(new_source, target);
                        int intersections = max_intersections(new_g);
                        if (intersections < min_intersections)
                        {
                            min_intersections = intersections;
                            best_node = new_source;
                        }
                    }
                }
            }

            if (min_intersections < worst_edge.first)
            {
                // We add the edge between the new source node and the target node
                G.newEdge(best_node, target);
                check_node[best_node] = true;
                check_node[source] = false;
            }
            else
            {
                // We add back the edge we removed at the beginning as
                // no better configuration was found
                G.newEdge(edge->source(), edge->target());
            }
        }
    }
}