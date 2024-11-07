#include "../include/algorithms/lineSweeper.hpp"
#include "../include/graphIntersect.hpp"
#include "../include/mathGeometry.hpp"

void Graph::UpdateNodeLocation(int id, int x, int y) {
    Node& node = this->getNode(id);
    node.setX(x);
    node.setY(y);
    
}
float Graph::getEdgeLength(const Edge &edge) {
    Node& source = this->getNode(edge.getSource_id());
    Node& target = this->getNode(edge.getTarget_id());
    VectorGeometry v(source, target);
    return getNorm(v);
}
const Edge& Graph::getHighestIntersection(){

        auto & maxPair = *std::max_element(
        this->edges.begin(), this->edges.end(),
        [](const std::pair<Edge, int>& a, const std::pair<Edge, int>& b) {
            return a.second < b.second;
        }
    );
    return maxPair.first;
}