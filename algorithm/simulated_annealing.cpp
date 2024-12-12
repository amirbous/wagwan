

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/algorithm/rearrangeGraph.hpp"

#include <set>
#include <limits.h>


void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, int max_iterations, int initial_area,
                            int width, int height)
{
    auto probability = [](int energy, int energy_new, double temperature) {
        return (energy_new < energy) ? 1.0 : std::exp(-(energy_new - energy) / temperature);
    };

    std::map<std::pair<int, int>, bool> check_node{};
    for (auto node : G.nodes)
    {
            check_node[{GA.x(node), GA.y(node)}] = true;
    }

    int iteration_count = 0, energy = 0, energy_new = 0;
    int highestCount = INT_MAX;
    double temperate = 0.0;
    std::map<int, ogdf::edge,  std::greater<int>> intersection_edges{},
                                                local_intersection_edges{};
    ogdf::edge worst_edge{};
    ogdf::node source{},
                target{};
    int source_x = 0, source_y = 0;
    double temperature = 0.0;
    while (iteration_count < max_iterations && highestCount > 0) {

        // extracting data about worst edge
        intersection_edges = calculate_singular_intersections(findIntersections(G, GA));
        worst_edge = intersection_edges.begin()->second;
        highestCount = intersection_edges.begin()->first;

        // del edge to replace
        G.delEdge(worst_edge);

        // start simulated annealing
        energy = highestCount;

        // extract potential new locations
        std::set<std::pair<int, int>> potential_sources;
        std::set<std::pair<int, int>> ring_sources = {{source_x, source_y}};
            
        for (int explore = 1; explore <= initial_area; explore++)
        {
            int addition = iteration_count + 1;
            std::set<std::pair<int, int>> new_potential_sources;
            // Iterate through last layer potential new positions
            for (auto last_source : ring_sources)
            {
                int temp_x = last_source.first;
                int temp_y = last_source.second;
                std::set<std::pair<int, int>> temp_potential_sources = {
                    {temp_x + addition, source_y},
                    {temp_x, temp_y + addition},
                    {temp_x + addition, temp_y + addition},
                    {temp_x - addition, temp_y - addition},
                    {temp_x - addition, temp_y},
                    {temp_x, temp_y - addition},
                    {temp_x + addition, temp_y - addition},
                    {temp_x - addition, temp_y + addition}
                };
                new_potential_sources.insert(temp_potential_sources.begin(), temp_potential_sources.end());
            }
            ring_sources.clear();
            ring_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
            potential_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
        }
        if (potential_sources.empty()) {
            std::cerr << "Error: No potential sources available!" << std::endl;
            continue;  // Or handle this case appropriately
        }

        // getting a random element from the potential list
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> distribution(0, potential_sources.size() - 1);
        int random_idx = distribution(gen);
        auto it = potential_sources.begin();
        std::advance(it, random_idx);
        auto new_source = *it;

        energy_new = 0;

        if (!check_node[new_source])
        {
            //ogdf::node new_node = G.newNode();
            GA.x(source) = new_source.first;
            GA.y(source) = new_source.second;
            ogdf::edge new_edge = G.newEdge(source, target);
            
            std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA);
            local_intersection_edges = calculate_singular_intersections(intersections);
            energy_new = calculate_specific_intersections(intersections, new_edge);

            // Safely delete the edge and node after testing

            G.delEdge(new_edge);

        }

        std::uniform_real_distribution<> distribution2(0.0, 1.0);
        double random_probability = distribution2(gen);

        if (probability(energy, energy_new, temperature) >= random_probability)
        {
            // We add the edge between the new source node and the target node

            check_node[{GA.x(source), GA.y(source)}] = false;

            GA.x(source) = new_source.first;
            GA.y(source) = new_source.second;

            check_node[new_source] = true;

            G.newEdge(source, target);

            
        }
        else
        {
            // Re-create the edge if the new configuration is not accepted
            G.newEdge(source, target);
        }

    }

}

   /* for (int iteration = 1; iteration <= max_iterations; iteration++)
    {
        double temperature = 1.0 - static_cast<double>(iteration + 1) / max_iterations;
        std::map<int, ogdf::edge,  std::greater<int>> intersection_edges = calculate_singular_intersections(findIntersections(G, GA));

        for (auto worst_edge : intersection_edges)
        {

            ogdf::edge edge = worst_edge.second;
            ogdf::node source = edge->source();
            ogdf::node target = edge->target();

            int source_x = GA.x(source);
            int source_y = GA.y(source);

            G.delEdge(edge);  // Safely delete the edge

            std::map<int, ogdf::edge,  std::greater<int>> best_intersection_edges;
            int energy = worst_edge.first;
            
            // Set that stores potential new positions for this source
            std::set<std::pair<int, int>> potential_sources;
            std::set<std::pair<int, int>> ring_sources = {{source_x, source_y}};
            
            for (int explore = 1; explore <= initial_area; explore++)
            {
                int addition = iteration + 1;

                std::set<std::pair<int, int>> new_potential_sources;

                // Iterate through last layer potential new positions
                for (auto last_source : ring_sources)
                {
                    int temp_x = last_source.first;
                    int temp_y = last_source.second;

                    std::set<std::pair<int, int>> temp_potential_sources = {
                        {temp_x + addition, source_y},
                        {temp_x, temp_y + addition},
                        {temp_x + addition, temp_y + addition},
                        {temp_x - addition, temp_y - addition},
                        {temp_x - addition, temp_y},
                        {temp_x, temp_y - addition},
                        {temp_x + addition, temp_y - addition},
                        {temp_x - addition, temp_y + addition}
                    };

                    new_potential_sources.insert(temp_potential_sources.begin(), temp_potential_sources.end());
                }

                ring_sources.clear();
                ring_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
                potential_sources.insert(new_potential_sources.begin(), new_potential_sources.end());
            }

            // Now we pick a random new potential position and calculate our probability function
            if (potential_sources.empty()) {
                std::cerr << "Error: No potential sources available!" << std::endl;
                continue;  // Or handle this case appropriately
            }

            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> distribution(0, potential_sources.size() - 1);
            int random_idx = distribution(gen);
            auto it = potential_sources.begin();
            std::advance(it, random_idx);

            auto new_source = *it;

            int energy_new = 0;
            if (!check_node[new_source])
            {
                ogdf::node new_node = G.newNode();
                GA.x(new_node) = new_source.first;
                GA.y(new_node) = new_source.second;
                ogdf::edge new_edge = G.newEdge(new_node, target);
                
                std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA);
                best_intersection_edges = calculate_singular_intersections(intersections);
                energy_new = calculate_specific_intersections(intersections, new_edge);

                // Safely delete the edge and node after testing
                G.delEdge(new_edge);
                G.delNode(new_node);
            }

            std::uniform_real_distribution<> distribution2(0.0, 1.0);
            double random_probability = distribution2(gen);

            if (probability(energy, energy_new, temperature) >= random_probability)
            {
                // We add the edge between the new source node and the target node
                ogdf::node new_node = G.newNode();
                GA.x(new_node) = new_source.first;
                GA.y(new_node) = new_source.second;

                G.newEdge(new_node, target);
                nodes_id[new_node] = nodes_id[target];
                nodes_id.erase(source);

                check_node[new_source] = true;
                check_node[{source_x, source_y}] = false;
            }
            else
            {
                // Re-create the edge if the new configuration is not accepted
                G.newEdge(source, target);
            }
        }*/



