#include "wordgraph.h"

void WordGraph::buildConnections(int index)
{
    // Check every word except yourself

    for (int i = 0; i < index; i++)
    {
        if (isLegalWeaverMove(*_nodes[i], *_nodes[index]))
        {
            (*_nodes[i]).addNeighbour(index);
            (*_nodes[index]).addNeighbour(i);
        }
    }
}

void WordGraph::buildConnectionsOneWay(int index)
{
    int max = _nodes.size();
    for (int i = 0; i < max; i++)
    {
        if (isLegalWeaverMove(*_nodes[i], *_nodes[index]))
        {
            (*_nodes[index]).addNeighbour(i); // note that we only modify the element which we are working on in this case!
        }
    }
}

std::vector<std::string> WordGraph::query(std::string const &source, std::string const &destination)
{
    /**
     * @brief Goal - assemble a vector of strings from source to destination using the graph structure and the Dijkstra algortihm
     *
     */
    int totalSize = _nodes.size();
    const int inf = totalSize + 100; // Infinity is disappointingly small

    std::vector<std::string> answer;
    if (source == destination)
        return answer;

    // Prepare the graph for pathfinding

    for (int i = 0; i < totalSize; i++)
    {
        (*_nodes[i]).setVisited(false);
        (*_nodes[i]).setDistance(inf);
        (*_nodes[i]).setSource(-1);
    }

    // Prepare the starting node

    (*_nodes[((*this)[source])]).setDistance(0);

    auto start = std::chrono::high_resolution_clock::now();

#if defined(PARALLEL)


    bool stop = false;

    std::vector<int> available_nodes = {(*this)[source]};

    unsigned int n = 4; // Thanks to the wonders of C++11 we can ask the system how many cores we have available
    std::vector<std::thread> threads;                     // We must keep track of our threads to later join them
    threads.reserve(n);

    while (!stop)
    {
        
    }

#else

    int current_node = (*this)[source];

    while (_nodes[current_node].getDistance() != inf)
    {
        // Do distance setting on unvisited neighbouring nodes

        Node &current = _nodes[current_node];

        int neighborsAmount = current.getNeighboursLength();
        int current_distance = current.getDistance();

        for (int i = 0; i < neighborsAmount; i++)
        {
            Node &neighbor = _nodes[(_nodes[current_node])[i]];

            if (neighbor.getVisited() == false && neighbor.getDistance() > current_distance + 1)
            {
                neighbor.setDistance(current_distance + 1);
                neighbor.setSource(current_node);
            }
        }

        // Done in this node, it will not be visited again
        current.setVisited(true);

        // Did we mark the word_b node as visited? If so, we're done and it's time to prepare the output
        if (current.getName() == destination)
        {

            int index = current_node;
            while (index != -1)
            {
                answer.push_back(_nodes[index].getName());
                index = _nodes[index].getSource();
            }

            auto end = std::chrono::high_resolution_clock::now();

            std::cout << "Processing the query took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds \n";

            return answer;
        }

        // If not, find the other node
        current_node = this->pickNextNode();
    }

#endif

    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Processing the query took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds \n";

    // If we reach here it means the node is unreachable

    return answer;
}

int WordGraph::pickNextNode()
{

    int totalSize = _nodes.size();
    int smallestNode = 0;

    // Pick one that wasn't visited
    int i = 0;
    for (; i < totalSize; i++)
    {
        if (_nodes[i].getVisited() == 0)
        {
            smallestNode = i;
            break;
        }
    }

    for (; i < totalSize; i++)
    {
        if (_nodes[i].getVisited() == 0 && _nodes[i].getDistance() < _nodes[smallestNode].getDistance())
        {
            smallestNode = i;
        }
    }

    return smallestNode;
}

int WordGraph::operator[](std::string const &searched)
{
    int totalSize = _nodes.size();
    for (int i = 0; i < totalSize; i++)
    {
        if (_nodes[i].getName() == searched)
            return i;
    }
    return -1;
}
