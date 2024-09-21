#include "algorithms.h"
#include <unordered_map>
#include <utility>
#include <QString>
#include <QDebug>
#include <QMessageBox>

std::string minDistance(std::unordered_map<std::string, int> &dist, std::unordered_map<std::string, bool> &sptSet) {
    int min = INT_MAX;
    std::string min_index;
    int n = dist.size();
    for (auto &it: dist)
        if (!sptSet[it.first] && it.second < min)
            min = it.second, min_index = it.first;
    return min_index;
}

std::list<std::string> Algorithms::Dijkstra(const Graph *graph, const std::string &start, const std::string &goal) {
    if (!graph->hasNode(start) || !graph->hasNode(goal))
        return std::list<std::string>();
    auto nodes = graph->nodeList();
    std::unordered_map<std::string, int> dist;
    for (auto &node: nodes)
        dist[node->name()] = INT_MAX;

    std::unordered_map<std::string, bool> sptSet;
    std::unordered_map<std::string, std::string> parent;
    dist[start] = 0;
    for (int count = 0; count < graph->countNodes() - 1; count++) {
        auto uname = minDistance(dist, sptSet);
        sptSet[uname] = true;
        for (auto &v: nodes) {
            if (!sptSet[v->name()] && graph->hasEdge(uname, v->name()) && dist[uname] != INT_MAX &&
                dist[uname] + graph->weight(uname, v->name()) < dist[v->name()]) {
                dist[v->name()] = dist[uname] + graph->weight(uname, v->name());
                parent[v->name()] = uname;
            }
        }
    }
    QString ans;
    std::list<std::string> path;
    //ans.append("Shortest path from " + start + " to " + goal + ": ");
    if (dist[goal] == INT_MAX) {
        std::cout << " not found!\n";
        return path;
    }
    auto curr = goal;
    while (!curr.empty()) {
        path.push_front(curr);
        curr = parent[curr];
    }
    for (auto &nodeName: path){
        //ans.append(nodeName + " ");
    }
    int a = static_cast<int>(dist[goal]);
    ans.append('\t' + "         cost = " + QString::number(a));
    QMessageBox::information(0,"Dijkstra",ans);
    return path;
}




std::list<std::pair<std::string, int>> Algorithms::getColoringResult(const Graph *graph, std::string source) {
    std::list<std::pair<std::string, int>> res_list;
    if (graph->isDirected())
        return res_list;

    auto nodeList = graph->nodeList();
    auto nodes = std::vector<Node *>(nodeList.begin(), nodeList.end());
    std::sort(nodes.begin(), nodes.end(),
              [](auto const &u, auto const &v) {
                  return u->undirDegree() > v->undirDegree();
              });
    std::unordered_map<std::string, int> colors;
    for (auto &node: nodes) {
        colors[node->name()] = -1;
    }
    for(int i = 0; i <nodes.size(); i++){
        std::cout << nodes[i] << std::endl;
    }
    colors[nodes.front()->name()] = 0;
    for (int i = 1; i < nodes.size(); ++i) {
        std::vector<int> usedColors(nodes.size(), false);
        for (auto& p: graph->edgeSet()) {
            auto u = p.first.first;
            auto v = p.first.second;
            if (v->name() == nodes[i]->name()) {
                std::swap(u, v);
            }
            if (u->name() == nodes[i]->name() && colors[v->name()] != -1) {
                usedColors[colors[v->name()]] = true;
            }
        }
        int v_color;
        for (v_color = 0; v_color < nodes.size(); ++ v_color) {
            if (!usedColors[v_color]) {
                break;
            }
        }
        colors[nodes[i]->name()] = v_color;
    }
    for (auto &node: nodeList) {
        res_list.push_back({node->name(), colors[node->name()]});
    }
    return res_list;
}

std::list<std::pair<std::string, std::string>> Algorithms::displayColoring(const Graph *graph, std::string source) {
    if (graph->isDirected())
        return std::list<std::pair<std::string, std::string>>();
    auto result = getColoringResult(graph, std::move(source));
    std::list<std::pair<std::string, std::string>> resultToSent;
    for (auto &it: result) {
        resultToSent.emplace_back(it.first, std::to_string(it.second));
    }
    return resultToSent;
}

void Algorithms::randomizeGraph(){
    const int MAX_NODE_SIZE = 7;
    std::string names[MAX_NODE_SIZE];

    for(int i = 0; i < MAX_NODE_SIZE; i++){
        names[i] = static_cast<char>(97 + i);
        names[i] += "0";
    }

    std::ofstream out;
    out.open("random_graph.txt", std::ofstream::out | std::ofstream::trunc);

    Random kek;
    auto isDirected = kek.drawNumber(0,1);
    auto isWeighted = kek.drawNumber(0,1);
    out << isDirected << " "<< isWeighted;

    auto number_of_node = kek.drawNumber(2,MAX_NODE_SIZE);
    out << '\n' << number_of_node << '\n';
    for(auto i = 0; i < number_of_node; i++){
        out << names[i] << " " << kek.drawNumber(0,500) << " " << kek.drawNumber(0,500) << '\n';
    }
    int edge_num;

    if(isDirected){
        edge_num = number_of_node * number_of_node - number_of_node;
    }
    else{
        edge_num = (number_of_node * number_of_node - number_of_node) / 2;
    }
    out << edge_num << '\n';

    for(int i = 0; i < edge_num; i++){
        auto first_node = kek.drawNumber(0,number_of_node - 1);
        auto second_node =  kek.drawNumber(0,number_of_node - 1);

        while(first_node == second_node){
            first_node = kek.drawNumber(0,number_of_node - 1);
            second_node =  kek.drawNumber(0,number_of_node - 1);
        }

        if(isWeighted){
            out << names[first_node] << " " << names[second_node]
                << " " << kek.drawNumber(1,8) << '\n';
        }
        else{
            out << names[first_node] << " " << names[second_node]
                << " 1" << '\n';
        }
    }

}
