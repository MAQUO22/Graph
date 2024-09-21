#ifndef GRAPHGRAPHICSSCENE_H
#define GRAPHGRAPHICSSCENE_H

#include "graph.h"
#include "edgegraphics.h"
#include "nodegraphics.h"
#include <QGraphicsScene>
#include <QtWidgets>

enum GraphDemoFlag {
    OnlyNode, OnlyEdge, EdgeAndNode, Component, Coloring
};

class GraphGraphicsScene : public QGraphicsScene {
    Q_OBJECT
public:
    GraphGraphicsScene();

    explicit GraphGraphicsScene(GraphType::Graph *graph);

    GraphType::Graph *graph() const;

    void setGraph(GraphType::Graph *graph);

    void clearAll();

    friend class GraphGraphicsView;

    ~GraphGraphicsScene() override;

    EdgeGraphicsItem *edgeItem(const std::string &uname, const std::string &vname);

    NodeGraphicsItem *nodeItem(const std::string &name);



public slots:

    void reload();

    void setInterval(int interval) {
        this->_interval = interval;
        if (_uniqueTimer)
            this->_uniqueTimer->setInterval(interval);
    }


signals:

    void graphChanged();

    void needRedraw();

private:

    using QGraphicsScene::clear;

    GraphType::Graph *_graph{};
    std::unordered_map<std::string, NodeGraphicsItem *> _nodeItems;
    std::unordered_map<std::pair<std::string, std::string>, EdgeGraphicsItem *> _edgeItems;

    std::unique_ptr<QTimer> _uniqueTimer;
    std::list<std::string> _listOfNode;
    std::list<std::list<std::string>> _listOfList;
    std::list<std::pair<std::string, std::string>> _listOfPair;
    int _interval;

};

#endif // GRAPHGRAPHICSSCENE_H
