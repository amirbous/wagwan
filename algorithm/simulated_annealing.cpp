

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


void simulated_annealing(ogdf::Graph &G, ogdf::GraphAttributes &GA, std::unordered_map<ogdf::node, int> &nodes_id, int max_iterations,
                            int width, int height, int cooling_technique, double initial_temperature, double cooling_rate)
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


        // extracting data about worst edge
        intersection_edges = calculate_singular_intersections(findIntersections(G, GA));
                

        // check if already no intersections are found :)
        if (intersection_edges.empty()) {
            std::cout << "REACHED 0 INTERSECTION WITHIN " << iteration_count << " ITERATIONS!" << std::endl;
            return;
        }
        // getting a random edge from the 4 first edges
        auto  lookupEdge_it = intersection_edges.begin();
        /*std::uniform_int_distribution<> dist(0, intersection_edges.size() - 1);
        int random_advance = dist(gen);
        std::advance(lookupEdge_it, random_advance);*/

        ogdf::edge worst_edge = lookupEdge_it->second;
        source = worst_edge->source();
        target = worst_edge->target();

        source_x = GA.x(source);
        source_y = GA.y(source);

        highestCount = lookupEdge_it->first;

        // del edge to replace
        G.delEdge(worst_edge);

        // start simulated annealing
        energy = highestCount;

        // random angle + random distance based on temperature
        int x,y;

        do
        {
            std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);
            std::uniform_int_distribution<> length_dist(0, static_cast<int>(temperature * width));

            double angle = angle_dist(gen);
            int length = length_dist(gen);
            /*if (iteration_count % 10 == 0) {
                std::cout << iteration_count << "  ITERATIONS HAVE PASSED!" << 
                                                ", LENGTH= " << length << 
                                                ", LOOKUP DISTANCE= " << temperature << 
                                                ", ANGLE= " << angle << std::endl;
            }*/


            double new_x = source_x + length * std::cos(angle);
            double new_y = source_y + length * std::sin(angle);

            x = static_cast<int>(std::round(new_x));
            y = static_cast<int>(std::round(new_y));
        }
        while (x < 0 || y < 0 || x >= width || y >= height );

        std::pair<int, int> new_source = {x, y};

        if (!check_node[new_source])
        {

            GA.x(source) = new_source.first;
            GA.y(source) = new_source.second;
            ogdf::edge new_edge = G.newEdge(source, target);

            std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA);
            energy_new = calculate_specific_intersections(intersections, new_edge);



            GA.x(source) = source_x;
            GA.y(source) = source_y;
            G.delEdge(new_edge);

            std::uniform_real_distribution<> distribution2(0.0, 1.0);
            double random_probability = distribution2(gen);

            if (probability(energy, energy_new, temperature) >= random_probability)
            {
                // we add the edge between the new source node and the target node

                check_node[{GA.x(source), GA.y(source)}] = false;

                GA.x(source) = new_source.first;
                GA.y(source) = new_source.second;

                check_node[new_source] = true;

                G.newEdge(source, target);

            }
            else
            {
                // re-create the edge if the new configuration is not accepted
                G.newEdge(source, target);
            }
        }
        else
        {
            G.newEdge(source,target);
        }
    iteration_count ++;
    }
    std::cout << "MAXIMUM NUMBER OF ITERATIONS ACHIEVED" << std::endl;

}



