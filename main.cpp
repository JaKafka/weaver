#include <iostream>
#include <vector>

#include "wordgraph.h"

int main(int argc, char** argv)
{
    /*
        Get words to work with from a file
    */

    WordGraph graph = WordGraph("../data/scrabble_words.txt", 4);
    //graph.printDebug();

    std::vector<std::string> answer = graph.query("halo", "glow");

    for (int i = answer.size()-1; i >= 0 ; i--)
    {
        std::cout << answer[i] << std::endl;
    }
    
    return 0;
}