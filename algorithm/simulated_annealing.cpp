#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node,int> &nodes_id, int iterations, int initial_area)
{
    std::map<std::pair<int,int>, bool> check_node;
    for(auto node: G.nodes)
    {
        check_node[{GA.x(node), GA.y(node)}] = true;
    }

    for (int iteration = 0; iteration < iterations; iteration++)
    {
        std::vector<std::pair<int, ogdf::edge>> intersection_edges = calculate_singular_intersections(findIntersections(G,GA));
        for(auto worst_edge: intersection_edges)
        {
            ogdf::edge edge = worst_edge.second;
            ogdf::node source = edge->source();
            ogdf::node target = edge->target();


            int source_x = GA.x(source);
            int source_y = GA.y(source);

            G.delEdge(edge);
            std::pair<int,int> best_node;
            std::vector<std::pair<int,ogdf::edge>> best_intersection_edges;
            int min_intersections = worst_edge.first;
            // set that stores potential new positions for this source
            std::set<std::pair<int,int>> potential_sources = {{source_x, source_y}};
            for (int explore = 1; explore <= initial_area; explore++)
            {
                // we search in the next layer + current iteration
                int addition = iteration+1;

                // set that stores the points of potential new sources considering the layer explore
                std::set<std::pair<int,int>> new_potential_sources;

                // iterate through last layer potential new positions
                for(auto last_source : potential_sources)
                {
                    int temp_x = last_source.first;
                    int temp_y = last_source.second;

                    std::set<std::pair<int,int>> temp_potential_sources = {
                        {temp_x+addition,source_y},
                        {temp_x, temp_y+addition},
                        {temp_x+addition,temp_y+addition},
                        {temp_x-addition,temp_y-addition},
                        {temp_x-addition,temp_y},
                        {temp_x, temp_y-addition},
                        {temp_x+addition, temp_y-addition},
                        {temp_x-addition, temp_y+addition}
                    };

                    new_potential_sources.insert(temp_potential_sources.begin(), temp_potential_sources.end());
                }

                // now we iterate over the set that contains the new potential positions ofr this layer
                // and calculate how our max crossing changed, in case it was improved, we save this as the best
                // current position
                for(auto new_source: new_potential_sources)
                {
                    if (!check_node[new_source])
                    {
                        ogdf::Graph new_G = G;
                        ogdf::GraphAttributes new_GA = GA;
                        ogdf::node new_node;
                        new_GA.x(new_node) = new_source.first;
                        new_GA.y(new_node) = new_source.second;
                        new_G.newEdge(new_node, target);
                        std::vector<std::pair<int,ogdf::edge>> intersecting_edges = calculate_singular_intersections(findIntersections(new_G,new_GA));
                        if (intersecting_edges[0].first < min_intersections)
                        {
                            min_intersections = intersecting_edges[0].first;
                            best_node = new_source;
                            best_intersection_edges = intersecting_edges;
                        }
                    }
                }
                potential_sources.clear();
                potential_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
            }

            if (min_intersections < worst_edge.first)
            {
                // We add the edge between the new source node and the target node
                ogdf::node new_source;
                GA.x(new_source) = best_node.first;
                GA.y(new_source) = best_node.second;
                G.newEdge(new_source, target);
                nodes_id[new_source] = nodes_id[source];
                nodes_id.erase(source);
                check_node[best_node] = true;
                check_node[{source_x,source_y}] = false;
                intersection_edges = best_intersection_edges;
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