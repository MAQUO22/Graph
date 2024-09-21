#ifndef ALGORITHMS_H
#define ALGORITHMS_H

#include "graph.h"
#include "random.h""
#include <queue>
#include <stack>
#include <list>

using namespace GraphType;

class Algorithms
{
private:
    Algorithms() = default;
public:
    static std::list<std::string> Dijkstra(const Graph *graph, const std::string &start, const std::string &goal);

    static std::list<std::pair<std::string, int>> getColoringResult(const Graph *graph, std::string source = "");

    static std::list<std::pair<std::string, std::string>> displayColoring(const Graph *graph, std::string source = "");

    static void randomizeGraph();
};

#endif // ALGORITHMS_H
