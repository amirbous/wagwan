

#include <set>
#include <ogdf/basic/Graph.h>
#include <ogdf/basic/graph_generators.h>
#include <ogdf/fileformats/GraphIO.h>
#include <ogdf/basic/GraphAttributes.h>

#include "../include/algorithm/simulated_annealing.hpp"
#include "../include/algorithm/linesweeper.hpp"
#include "../include/intersectGraph.hpp"

#include <set>
#include <limits.h>

std::pair<int,int> generate_new_source(std::mt19937 &gen, double &temperature, int &source_x, int &source_y, int &width, int &height)
{
    int x,y;
    do
    {
        std::uniform_real_distribution<> angle_dist(0, 2 * M_PI);
        std::uniform_int_distribution<> length_dist(0, static_cast<int>(temperature * 50));

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

    return {x,y};
}

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
        if (iteration_count % 200 == 0) std::cout << "In iteration number= " << iteration_count << std::endl;
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


        ogdf::edge worst_edge = lookupEdge_it->second;


        source = worst_edge->source();
        target = worst_edge->target();


        // Usefull later for evaluating the energy, as only the incident edges will be changed locally,
        // However global evaluation in outer loop requires the full edge list from the graph
        std::vector<ogdf::edge> source_incident_edges = getIncidentEdges(G, GA, source);
        source_x = GA.x(source);
        source_y = GA.y(source);

        highestCount = calculate_specific_intersections(findIntersections(G,GA), worst_edge);

        // start simulated annealing
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

                // to reduce time for eavluating edge intersections, instead of evaluating whole edge, just the edge with the worst 
                
                std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G, GA, source_incident_edges);

                energy_new = calculate_singular_intersections(intersections).begin()->first;



                std::uniform_real_distribution<> distribution2(0.0, 1.0);
                double random_probability = distribution2(gen);

                if (probability(energy, energy_new, temperature) >= random_probability)
                {
                    // we add the edge between the new source node and the target node
                    check_node[{source_x, source_y}] = false;
                    check_node[new_source] = true;
                    check = true;
                }
                else
                {
                    // re-create the edge if the new configuration is not accepted
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

        //G.newEdge(source,target);
        iteration_count ++;
    }
    std::cout << "MAXIMUM NUMBER OF ITERATIONS ACHIEVED" << std::endl;

}






        /*if (iteration_count%2!=0)
        {
            std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G,GA);
            for (auto intersection : intersections)
            {
                if (intersection.first == worst_edge)
                {
                    worst_edge = intersection.second;
                    break;
                }

                if (intersection.second == worst_edge)
                {
                    worst_edge = intersection.first;
                    break;
                }
            }
        }*/

        /*if(iteration_count % 2 == 0)
        {
            source = worst_edge->source();
            target = worst_edge->target();
        }
        else
        {
            std::vector<std::pair<ogdf::edge, ogdf::edge>> intersections = findIntersections(G,GA);
            for (auto intersection : intersections)
            {
                if (intersection.first == worst_edge)
                {
                    source = intersection.second->source();
                    target = intersection.second->target();
                    break;
                }

                if (intersection.second == worst_edge)
                {
                    source = intersection.first->source();
                    target = intersection.first->target();
                    break;
                }
            }
        }*/
