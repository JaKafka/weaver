#include "node.h"

void Node::addNeighbour(int node)
{
    _neighbours.push_back(node);
}
int Node::getNeighboursLength() const
{
    return _neighbours.size();
}
int Node::operator[](int index) const
{
    return _neighbours[index];
}

std::string &Node::getName()
{
    return _name;
}

bool isLegalWeaverMove(Node const &a, Node const &b)
{
    int diff = 0;
    for (int i = 0; i < a._name.size(); i++)
    {
        if ((a._name)[i] != (b._name)[i])
            diff++;
        if (diff > 1)
            return false;
    }
    return true;
}

bool Node::getVisited() const
{
    return _visited;
}
void Node::setVisited(bool a)
{
    _visited = a;
}
int Node::getDistance() const
{
    return _distance;
}
void Node::setDistance(int a)
{
    _distance = a;
}
void Node::setSource(int a)
{
    _source = a;
}
int Node::getSource() const
{
    return _source;
}
