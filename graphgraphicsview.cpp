#include "graphgraphicsview.h"
#include <QDebug>

GraphGraphicsView::GraphGraphicsView() {
    setDragMode(ScrollHandDrag);
    _moving = false;
    _selectTargetNode = false;
    _isRunningAlgoDemo = false;
    _currentScale = 1.;
}

void GraphGraphicsView::wheelEvent(QWheelEvent *event) {
    QGraphicsView::wheelEvent(event);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    double scaleFactor = 1.05;
    if (event->angleDelta().y() > 0 && _currentScale <= _scaleMax) {
        scale(scaleFactor, scaleFactor);
        _currentScale *= scaleFactor;
    } else {
        scale(1 / scaleFactor, 1 / scaleFactor);
        _currentScale /= scaleFactor;
    }
}

void GraphGraphicsView::contextMenuEvent(QContextMenuEvent *event) {

}

void GraphGraphicsView::mousePressEvent(QMouseEvent *event) {
    for (auto si: scene()->selectedItems()) {
        auto ni = dynamic_cast<NodeGraphicsItem *>(si);
        if (ni) ni->setOnSelectedColor(ni->defaultOnSelectedColor());
        else {
            auto ai = dynamic_cast<EdgeGraphicsItem *>(si);
            if (ai) ai->setOnSelectedColor(EdgeGraphicsItem::defaultOnSelectedColor());
        }
    }

    auto graphScene = dynamic_cast<GraphGraphicsScene *>(scene());
    if (graphScene->_uniqueTimer) {
        graphScene->_uniqueTimer->stop();
    }
    //graphScene->resetAfterDemoAlgo();

    if (!_selectTargetNode)
        QGraphicsView::mousePressEvent(event);
    if (_selectTargetNode) {
        QList<QGraphicsItem *> itemsTo = items(event->pos());
        if (!itemsTo.empty()) {
            auto *castedItemTo = dynamic_cast<NodeGraphicsItem *>(itemsTo[0]);
            if (_startItem && castedItemTo && _startItem != castedItemTo) {
                castedItemTo->setSelected(false);
                emit edgeSet(_startItem->node()->name(), castedItemTo->node()->name());
            }
        }
    }
    _selectTargetNode = false;
    this->_startItem = nullptr;
    scene()->update();
    viewport()->update();
}

void GraphGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    if (!scene()->selectedItems().empty()) {
        auto fni = dynamic_cast<NodeGraphicsItem *>(scene()->selectedItems()[0]);
        auto fai = dynamic_cast<EdgeGraphicsItem *>(scene()->selectedItems()[0]);
        if (fai) emit edgeSelected(fai->edge().u()->name(), fai->edge().v()->name());
        else if (fni) emit nodeSelected(fni->node()->name());
    }
    if (items(event->pos()).empty())
        emit unSelected();
    QGraphicsView::mouseReleaseEvent(event);
}

void GraphGraphicsView::setScene(GraphGraphicsScene *scene) {
    connect(scene, SIGNAL(graphChanged()), this, SLOT(redraw()));
    connect(scene, SIGNAL(needRedraw()), this, SLOT(redraw()));
    QGraphicsView::setScene(scene);
}

void GraphGraphicsView::redraw() {
    this->viewport()->update();
}
