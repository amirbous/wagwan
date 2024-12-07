#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node,int> &nodes_id, int iterations, int initial_area)
{
    auto probability = [](int energy, int energy_new, double temperature) {
        return (energy_new < energy) ? 1.0 : std::exp(-(energy_new - energy) / temperature);
    };

    std::map<std::pair<int,int>, bool> check_node;
    for(auto node: G.nodes)
    {
        check_node[{GA.x(node), GA.y(node)}] = true;
    }

    for (int iteration = 1; iteration <= iterations; iteration++)
    {
        double temperature = 1.0 - static_cast<double>(iteration + 1) / iterations;
        std::map<ogdf::edge, int> intersection_edges = calculate_singular_intersections(findIntersections(G,GA));
        for(auto worst_edge: intersection_edges)
        {
            ogdf::edge edge = worst_edge.first;
            ogdf::node source = edge->source();
            ogdf::node target = edge->target();

            int source_x = GA.x(source);
            int source_y = GA.y(source);

            G.delEdge(edge);
            std::map<ogdf::edge, int> best_intersection_edges;
            int energy = worst_edge.second;
            // set that stores potential new positions for this source
            std::set<std::pair<int,int>> potential_sources;
            std::set<std::pair<int,int>> ring_sources = {{source_x, source_y}};
            for (int explore = 1; explore <= initial_area; explore++)
            {
                // we search in the next layer + current iteration
                int addition = iteration+1;

                // set that stores the points of potential new sources considering the layer explore
                std::set<std::pair<int,int>> new_potential_sources;

                // iterate through last layer potential new positions
                for(auto last_source : ring_sources)
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

                ring_sources.clear();
                ring_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
                potential_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
            }

            // now we pick a random new potential position
            // and calculate our probability function
            if (potential_sources.empty()) {
                std::cerr << "Error: No potential sources available!" << std::endl;
                continue; // Or handle this case appropriately
            }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distribution(0, potential_sources.size()-1);
            int random_idx = distribution(gen);
            auto it = potential_sources.begin();
            std::advance(it, random_idx);

            std::pair<int,int> new_source = *it;

            int energy_new = 0;
            if (!check_node[new_source])
            {
                ogdf::Graph new_G = G;
                ogdf::GraphAttributes new_GA = GA;
                ogdf::node new_node = new_G.newNode();
                new_GA.x(new_node) = new_source.first;
                new_GA.y(new_node) = new_source.second;
                ogdf::edge new_edge = new_G.newEdge(new_node, target);;
                std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(new_G,new_GA);
                best_intersection_edges = calculate_singular_intersections(intersections);
                energy_new = calculate_specific_intersections(intersections,new_edge);
            }

            std::uniform_real_distribution<> distribution2(0.0, 1.0);
            double random_probability = distribution2(gen);

            if(probability(energy, energy_new, temperature) >= random_probability)
            {
                // We add the edge between the new source node and the target node
                ogdf::node new_node = G.newNode();
                GA.x(new_node) = new_source.first;
                GA.y(new_node) = new_source.second;
                G.newEdge(new_node, target);
                nodes_id[new_node] = nodes_id[source];
                nodes_id.erase(source);
                check_node[new_source] = true;
                check_node[{source_x,source_y}] = false;
                intersection_edges = best_intersection_edges;
            }
            else
            {
                // We add back the edge we removed at the beginning as
                // no better configuration was found
                G.newEdge(source, target);
            }
        }
    }
}

double probability(int energy, int energy_new, double temperature)
{
    if (energy_new < energy) return 1;
    return std::exp(-(energy_new - energy)/temperature);
}