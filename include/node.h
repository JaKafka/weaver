#pragma once

#include <string>
#include <iostream>
#include <vector>
#include <mutex>

#define PARALLEL

/**
 * @brief A single weaver word
 *
 */
class Node
{
    friend bool isLegalWeaverMove(Node const &a, Node const &b);

public:
    Node(std::string name, int distanceMax, int id) : _name(name), _visited(false), _neighbours(), _source(-1), _id(id), _distance(distanceMax) {}
    
    /**
     * @brief Add a neighbour pointer to the neighbours table
     * 
     * @param node 
     */
    void addNeighbour(int node);
    
    /**
     * @brief Return the neighbour at index
     * 
     * @param index 
     * @return Node* 
     */
    int operator[](int index) const;
    int getNeighboursLength() const;
    std::string &getName();
    bool getVisited() const;
    void setVisited(bool a);
    int getDistance() const;
    void setDistance(int a);
    void setSource(int a);
    int getSource() const;

    ~Node()
    {
        _neighbours.clear();
    }

private:
    // Data for this node
    std::string _name;
    // Structure information
    std::vector<int> _neighbours;
    // Dijkstra metadata
    int _id;
    bool _visited;
    int _distance;
    int _source;

    #if defined(PARALLEL)
        std::mutex node_lock;
    #endif
};

bool isLegalWeaverMove(Node const &a, Node const &b);