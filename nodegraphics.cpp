#include "nodegraphics.h"
#include "GraphGraphicsScene.h"
#include <QtWidgets>
#include <QDebug>
#include <utility>


NodeGraphicsItem::NodeGraphicsItem(GraphGraphicsScene *scene, GraphType::Node *node, QColor color) {
    this->_gscene = scene;
    this->_defaultColor = color;
    this->_color = _defaultColor;
    this->_radius = 80;
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setAcceptHoverEvents(true);
    setNode(node);
    _isMoving = false;
    this->_defaultSelectedColor = QColor(0,255,0);
    _selectedColor = _defaultSelectedColor;
}

void NodeGraphicsItem::setNode(GraphType::Node *node) {
    this->_node = node;
    this->setPos(_node->euclidePos());
    QFontMetrics fm(QFont(_font, _fontSize));
    _radius = std::max(_radius, fm.horizontalAdvance(QString::fromStdString(_node->name() + "  ")));
}

GraphType::Node *NodeGraphicsItem::node() const {
    return this->_node;
}

QRectF NodeGraphicsItem::boundingRect() const {
    return {-_radius / 2. + .5,
        -_radius / 2. + .5,
        static_cast<qreal>(_radius + 4),
        static_cast<qreal>(_radius + 4)
    };
}

const QList<QColor> &NodeGraphicsItem::colorTable() {
    static QList<QColor> colorTable = {
        QColor(255,255,255),
        QColor(255,0,0),
        QColor(255,165,0),
        QColor(255,255,0),
        QColor(0,128,0),
        QColor(0,0,255),
        QColor(128,128,128),
        QColor(135, 206, 235),
        QColor(240, 230, 140),
        QColor(255, 165, 0),
        QColor(240, 255, 255),
        QColor(0, 255, 255),
        QColor(240, 255, 255),
        QColor(255, 228, 225),
        QColor(220, 220, 220),
        QColor(176, 196, 222)
    };
    return colorTable;
};

QColor NodeGraphicsItem::defaultColor() { return _defaultColor; }

QColor NodeGraphicsItem::defaultOnSelectedColor() { return _defaultSelectedColor; }


static QColor setDefaultOnSelectedColor();

void NodeGraphicsItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    Q_UNUSED(option);
    Q_UNUSED(widget);
    if (isSelected())
        _color = defaultOnSelectedColor();
    else
        _color = defaultColor();
    painter->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    painter->setBrush(QBrush(_color));
    painter->drawEllipse(-_radius / 2, -_radius / 2, _radius, _radius);

    QFont font(_font, _fontSize);
    painter->setFont(font);
    QString txt = QString::fromStdString(this->node()->name());
    QFontMetrics fm(font);
    painter->drawText(-fm.horizontalAdvance(txt) / 2, fm.height() / 3, txt);
}

void NodeGraphicsItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event) {
    if (QLineF(event->screenPos(), event->buttonDownScreenPos(Qt::LeftButton))
            .length() < QApplication::startDragDistance()) {
        return;
    }
    _isMoving = true;
    setCursor(Qt::ClosedHandCursor);
    setPos(event->scenePos());
    this->node()->setEuclidePos(this->pos());
    emit _gscene->needRedraw();
    emit positionChanged();
    QGraphicsItem::mouseMoveEvent(event);
}

void NodeGraphicsItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsItem::mouseReleaseEvent(event);
    if (_isMoving) {
        setSelected(false);
        _isMoving = false;
    }
    emit this->_gscene->graphChanged();
    setCursor(Qt::OpenHandCursor);
}

void NodeGraphicsItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    QGraphicsItem::hoverEnterEvent(event);
    setCursor(Qt::PointingHandCursor);
}

QColor NodeGraphicsItem::onSelectedColor() const {
    return this->_selectedColor;
}
