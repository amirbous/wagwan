

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"
#include "../include/RTree.h"
#include "../include/structs.hpp"

#include <set>
#include <limits.h>

#include <csignal>

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"

#include "../include/IO.hpp"

#include <set>
#include <limits.h>




std::pair<int,int> generate_new_source(std::mt19937 &gen, double &temperature, int &source_x, int &source_y, int &width, int &height)
{
    int x,y;
    do
    {
        std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);
        std::uniform_int_distribution<> length_dist(0, static_cast<int>(width / 2));

        double angle = angle_dist(gen);
        int length = length_dist(gen);


        double new_x = source_x + length * std::cos(angle);
        double new_y = source_y + length * std::sin(angle);

        x = static_cast<int>(std::round(new_x));
        y = static_cast<int>(std::round(new_y));
    }
    while (x < 0 || y < 0 || x >= width || y >= height );

    return {x,y};
}

void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, 
                            std::unordered_map<ogdf::node, int> &nodes_id, int max_iterations,
                            int width, int height, int cooling_technique, double initial_temperature, 
                            double cooling_rate, int bestCount, const std::string &filePath)
{



	const static unsigned int seed = 42;
    std::mt19937 gen(seed);

    auto probability = [](double energy, double energy_new, double temperature) {
        return (energy_new < energy) ? 1.0 : (temperature == 0 ? 0 : std::exp(-(energy_new - energy) / temperature));
    };
    


    std::map<std::pair<int, int>, bool> check_node{};
    for (auto node : G.nodes)
    {
            check_node[{GA.x(node), GA.y(node)}] = true;
    }

    int iteration_count = 0;
    double energy = 0, energy_new = INT_MAX;
    int highestCount = INT_MAX;
    std::map<int, ogdf::edge,  std::greater<int>> intersection_edges{},
                                                local_intersection_edges{};
    ogdf::node source{},
                target{};
    int source_x = 0, source_y = 0;
    double temperature;
    while (iteration_count < max_iterations && highestCount > 0) {
        


        if (cooling_technique == 0)
            temperature = 1.0 - static_cast<double>(iteration_count + 1) / max_iterations;
        else if (cooling_technique == 1)
            temperature = initial_temperature / log(static_cast<double>(iteration_count + 2));
        else temperature = initial_temperature * std::pow(cooling_rate, iteration_count);


        intersection_edges = calculate_singular_intersections(findIntersections(G, GA));
                

        if (intersection_edges.empty()) {
            std::cout << "REACHED 0 INTERSECTION WITHIN " << iteration_count << " ITERATIONS!" << std::endl;
            return;
        }

        auto  lookupEdge_it = intersection_edges.begin();


        ogdf::edge worst_edge = lookupEdge_it->second;


        source = worst_edge->source();
        target = worst_edge->target();


        std::vector<ogdf::edge> source_incident_edges = getIncidentEdges(G, GA, source);
        source_x = GA.x(source);
        source_y = GA.y(source);

        highestCount = intersection_edges.begin()->first;
        if (iteration_count % 30 == 0) std::cout << "In iteration number= " << iteration_count
            << " - " << highestCount << std::endl;

        if (highestCount < bestCount) {
            writeGraphToJson(G, GA, filePath, nodes_id, width, height);
            bestCount = highestCount;
        }


        energy = highestCount;


        std::vector<std::pair<int, int>> new_sources_vector;
        while(new_sources_vector.size() < 20)
            new_sources_vector.push_back(generate_new_source(gen, temperature, source_x, source_y, width, height));

        bool check = false;
        for(auto new_source: new_sources_vector)
        {
            if (check)
                break;
            GA.x(source) = new_source.first;
            GA.y(source) = new_source.second;
            if (!check_node[new_source] && !check_node_on_anyEdge(G,GA,source)
                    && !edgeIntersects_anyNode(G, GA, worst_edge) && !nodeEdges_intersect_anyNode(G, GA, source))
            {


                std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA, source_incident_edges);

                energy_new = calculate_singular_intersections(intersections).begin()->first;



                std::uniform_real_distribution<> distribution2(0.0, 1.0);
                double random_probability = distribution2(gen);

                if (probability(energy, energy_new, temperature) >= random_probability)
                {
                    check_node[{source_x, source_y}] = false;
                    check_node[new_source] = true;
                    check = true;
                }
                else
                {
                    GA.x(source) = source_x;
                    GA.y(source) = source_y;
                }
            }
            else
            {
                GA.x(source) = source_x;
                GA.y(source) = source_y;
            }
        }


        iteration_count ++;
    }
    std::cout << "Best number achieved" << bestCount << std::endl;
    std::cout << "MAXIMUM NUMBER OF ITERATIONS ACHIEVED" << std::endl;

}


