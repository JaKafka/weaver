#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "node.h"
#include <chrono>
#include <thread>
#include <latch>
#include <mutex>
#include <barrier>
#include <memory>

#define PARALLEL

/**
 * @brief Container for all weaver words in a graph
 *
 */
class WordGraph
{

public:
    WordGraph(std::string filename, int desiredLength)
    {

        std::ifstream file;
        file.open(filename);
        if (!file)
        {
            std::cout << "An error has occured and the words file could not be opened." << std::endl;
            throw std::invalid_argument("The file could not be opened");
        }

        std::string buffer;
        file >> buffer;
        int word = 0;

        auto start = std::chrono::high_resolution_clock::now();

#if defined(PARALLEL)

        {
            std::vector<std::string> string_buffer;
            // In this case we filter out the words first, then build the graph
            while (!file.eof())
            {
                if (buffer.length() == desiredLength)
                {
                    string_buffer.push_back(buffer);
                     // dictSize just has to be large enough to be considered 'infinity' from the algorithm's point of view
                    word++;
                }
                file >> buffer;
            }

            _nodes.reserve(word);

            for (int i = 0; i < word; i++)
            {
                _nodes[i] = std::make_unique<Node>(buffer, -1, i);
            }
        }
        file.close();

        // We have the whole list now, building connections can begin
        // But how do we determine how many threads is enough?
        // One way - we could just input a static number, which would produce same results on any hardware
        // However, we want the maximum performance here!

        unsigned int n = std::thread::hardware_concurrency(); // Thanks to the wonders of C++11 we can ask the system how many cores we have available

        std::cout << "We have " << _nodes.size() << " elements in the graph and " << n << " threads available to do our work." << std::endl;

        int chunk = _nodes.size() / n; // This is a single chunk of data a process will work on -- this division of work could be organised in different ways, but we apply KISS here

        std::vector<std::thread> threads; // We must keep track of our threads to later join them
        threads.reserve(n);

        // Now the fun begins -- we'll use a C++20 feature to sync our work -- std::latch
        std::latch build_done{n};

        for (int i = 0; i < n; i++)
        {
            // We have figure out which indices our thread will work on
            size_t starting = i * chunk;
            size_t ending = (i + 1) * chunk;
            ending = std::min(ending, _nodes.size());
            std::cout << "Thread number " << i << " starting work from index " << starting << " to index " << ending << std::endl;

            // Now we make our thread, back in ye old days we'd need a function that it'd call, but then for every kind of thread that is used once we'd need its own function, with its own name...
            // Lambda!
            threads.push_back(std::thread{[&build_done, starting, ending, i, this]() -> void
                                          {
                                              for (int j = starting; j < ending; j++)
                                                  this->buildConnectionsOneWay(j);
                                              std::cout << i << " is finished..." << std::endl;
                                              build_done.count_down();
                                          }});
        }

        // We wait with proceeding until all processing is finished
        build_done.wait();

        // We have to join our threads back
        for (std::thread &th : threads)
        {
            th.join();
        }

        // And that's all! Easy as that!

#else

        // Words are loaded, filtered to be 4 letters long and as they are loaded their connections in the word graph are added
        while (!file.eof())
        {
            if (buffer.length() == desiredLength)
            {
                _nodes.push_back(Node(buffer, -1, word)); // dictSize just has to be large enough to be considered 'infinity' from the algorithm's point of view
                this->buildConnections(word);
                word++;
            }
            file >> buffer;
        }
        file.close();

#endif

        auto end = std::chrono::high_resolution_clock::now();

        std::cout << "Construction of the graph took: " << std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() << " microseconds \n";

        int dictionarySize = _nodes.size();
        for (int i = 0; i < dictionarySize; i++)
        {
            (*_nodes[i]).setDistance(dictionarySize + 100);
        }
    }

    // Add pointers to node
    void buildConnections(int index);
    void buildConnectionsOneWay(int index);
    int operator[](std::string const &searched);
    std::vector<std::string> query(std::string const &source, std::string const &destination);

    ~WordGraph()
    {
    }

    int pickNextNode();

private:

    #if defined(PARALLEL)
        std::vector<std::unique_ptr<Node>> _nodes;
    #else
        std::vector<Node> _nodes;
    #endif
};
