#include "route_planner.h"
#include <algorithm>
#include <iostream>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y)
    : m_Model(model) {
    // Convert inputs to percentage:
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;

    // Find closest nodes to start and end:
    start_node = &m_Model.FindClosestNode(start_x, start_y);
    end_node = &m_Model.FindClosestNode(end_x, end_y);
}

float RoutePlanner::CalculateHValue(RouteModel::Node const *node) {
    // Use the distance method from the Node class
    return node->distance(*end_node);
}

void RoutePlanner::AddNeighbors(RouteModel::Node *current_node) {
    // Populate the neighbors vector for the current node
    current_node->FindNeighbors();
    
    for (auto neighbor : current_node->neighbors) {
        neighbor->parent = current_node;
        neighbor->g_value = current_node->g_value + current_node->distance(*neighbor);
        neighbor->h_value = CalculateHValue(neighbor);
        
        // Only add to open_list if not visited
        if (!neighbor->visited) {
            neighbor->visited = true;
            open_list.push_back(neighbor);
        }
    }
}

RouteModel::Node *RoutePlanner::NextNode() {
    // Initialize a variable to store the index of the best node
    int idx = 0;

    // Iterate through the open list to find the node with the lowest f-value (g + h)
    for (size_t i = 1; i < open_list.size(); ++i) {
        if ((open_list[i]->g_value + open_list[i]->h_value) < 
            (open_list[idx]->g_value + open_list[idx]->h_value)) {
            idx = i; // Update the index of the best node
        }
    }

    // Retrieve the best node and remove it from the open list
    RouteModel::Node* best_node = open_list[idx];
    open_list.erase(open_list.begin() + idx);

    return best_node;
}


std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node *current_node) {
    std::vector<RouteModel::Node> path_found;

    // Traverse from current (end) node to the start node
    while (current_node != nullptr) {
        path_found.push_back(*current_node);
        current_node = current_node->parent;
    }

    // Reverse the path to go from start to goal
    std::reverse(path_found.begin(), path_found.end());

    // Calculate the total distance of the path by summing distances
    float total_distance = 0.0f;
    for (size_t i = 1; i < path_found.size(); ++i) {
        total_distance += path_found[i].distance(path_found[i - 1]);  // Pass the previous node as an argument
    }
    
    // Scale the total distance by the map's metric scale
    total_distance *= m_Model.MetricScale();
    distance = total_distance;

    return path_found;
}

void RoutePlanner::AStarSearch() {
    start_node->visited = true;
    open_list.push_back(start_node);

    while (!open_list.empty()) {
        RouteModel::Node *current_node = NextNode();
        
        if (current_node == end_node) {
            m_Model.path = ConstructFinalPath(current_node);
            return;
        }
        
        AddNeighbors(current_node);
    }
    
    // If open_list is empty and we haven't found the end_node, no path exists
    std::cout << "No path found!" << std::endl;
}