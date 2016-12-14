#include <QSGGeometry>
#include "SnakeGlobal.h"

BaseShape::BaseShape(const QRectF &rect)
    : m_anchios()
{
    m_anchios = rect;
}

void BaseShape::addVertex(QSGGeometry *key, QPointF *point)
{
    if(m_lstVertex.contains(key)) {
        QPointF *pointTemp = new QPointF(point);
        m_lstVertex.value(key).append(pointTemp);
    }
}

void BaseShape::cleanVertex(QSGGeometry *key)
{
    if(m_lstVertex.contains(key)) {
        m_lstVertex.value(key).clear();
    }
}

RectangleShape::RectangleShape()
    : m_containColor(Qt::cyan)
    , m_boundColor(Qt::red)
{

}

void RectangleShape::updateVertex(QSGGeometry *key)
{

}
