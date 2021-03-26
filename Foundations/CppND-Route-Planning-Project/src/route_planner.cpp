#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    RoutePlanner::start_node = &(m_Model.FindClosestNode(start_x, start_y));
    RoutePlanner::end_node = &(m_Model.FindClosestNode(end_x, end_y));
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    return node->distance(*end_node);
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    current_node->FindNeighbors();

    for (RouteModel::Node *N: current_node->neighbors){
        if (!N->visited){
            N->parent = current_node;
            N->h_value = CalculateHValue(N);
            N->g_value = current_node->g_value + N->distance(*current_node);
            N->visited = true;
            open_list.emplace_back(N);
        }
    }
}

bool Compare(const RouteModel::Node *n1, const RouteModel::Node *n2) {
  return (n1->g_value + n1->h_value) < (n2->g_value +n2->h_value); 
}


RouteModel::Node *RoutePlanner::NextNode() {
    std::sort(open_list.begin(), open_list.end(), Compare);
    RouteModel::Node *best_node = open_list[0];
    open_list.erase(open_list.begin());
    return best_node;
}

std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    // Create path_found vector
    distance = 0.0f;
    std::vector<RouteModel::Node> path_found;

    RouteModel::Node tmp = *current_node;
    while (1){
        if (tmp.parent != nullptr){
            distance += tmp.distance(*tmp.parent);
            path_found.emplace(path_found.begin(), tmp);
            tmp = *tmp.parent;
        }
        else{
            path_found.emplace(path_found.begin(), tmp);
            break;
        }
    }

    distance *= m_Model.MetricScale(); // Multiply the distance by the scale of the map to get meters.
    return path_found;

}

void RoutePlanner::AStarSearch() {
    RouteModel::Node *current_node = nullptr;

    start_node->visited = true;
    open_list.push_back(start_node);
    while (!open_list.empty()){
        current_node = NextNode();
        if (current_node->x == end_node->x && current_node->y == end_node->y){
            m_Model.path = ConstructFinalPath(current_node);
            break;
        }
        AddNeighbors(current_node);
    }
}