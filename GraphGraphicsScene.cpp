#include "GraphGraphicsScene.h"
#include "random.h"
#include <QDebug>
#include <memory>
#include <utility>

GraphGraphicsScene::GraphGraphicsScene() = default;

GraphGraphicsScene::GraphGraphicsScene(GraphType::Graph *graph) {
    setGraph(graph);
}

GraphType::Graph *GraphGraphicsScene::graph() const {
    return this->_graph;
}

void GraphGraphicsScene::setGraph(GraphType::Graph *graph) {
    this->_graph = graph;
    reload();
}

void GraphGraphicsScene::reload() {
    this->clearAll();
    for (const auto &node: graph()->nodeList())
        _nodeItems[node->name()] = new NodeGraphicsItem(this, node);
    for (auto it = graph()->edgeSet().begin(); it != graph()->edgeSet().end(); ++it) {
        auto edge = GraphType::Edge(it);
        _edgeItems.insert({std::make_pair(edge.u()->name(), edge.v()->name()),
            new EdgeGraphicsItem(this,
                                 _nodeItems[edge.u()->name()],
                                 _nodeItems[edge.v()->name()])
        });
    }
    for (const auto &item: _edgeItems)
        this->addItem(item.second);
    for (const auto &item: _nodeItems)
        this->addItem(item.second);
    this->update();
}


EdgeGraphicsItem *GraphGraphicsScene::edgeItem(const std::string &uname, const std::string &vname) {
    if (_edgeItems.find(std::make_pair(uname, vname)) != _edgeItems.end())
        return _edgeItems.find(std::make_pair(uname, vname))->second;
    else if (_graph->isUndirected() && _edgeItems.find(std::make_pair(vname, uname)) != _edgeItems.end())
        return _edgeItems.find(std::make_pair(vname, uname))->second;
    return nullptr;
}

NodeGraphicsItem *GraphGraphicsScene::nodeItem(const std::string &name) {
    if (_nodeItems.find(name) != _nodeItems.end())
        return _nodeItems[name];
    return nullptr;
}

void GraphGraphicsScene::clearAll() {
    this->clear();
    this->_nodeItems.clear();
    this->_edgeItems.clear();
}

GraphGraphicsScene::~GraphGraphicsScene() {
    this->clearAll();
}
