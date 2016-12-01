#include <QSGNode>
#include <QSGFlatColorMaterial>
#include "SnakeGlobal.h"

Cell::Cell()
{
    setFlag(ItemHasContents, true);
}

Cell::Cell(const Cell &cell)
{
    setFlag(ItemHasContents, true);
    setX(cell.x());
    setY(cell.y());

    m_iSize = cell.size();
    m_eShape = cell.shape();
    updateVertex();
}

Cell::~Cell()
{

}

void Cell::setShape(Cell::ENUM_ESHAPE shape)
{
    ENUM_ESHAPE shapeBak = m_eShape;
    m_eShape = shape;

    updateVertex();
    emit notifyShapeChanged(m_eShape, shapeBak);
}

Cell::ENUM_ESHAPE Cell::shape() const
{
    return m_eShape;
}

void Cell::setSize(int size)
{
    int sizeBak = m_iSize;
    m_iSize = size;

    updateVertex();
    emit notifySizeChanged(m_iSize, sizeBak);
}

int Cell::size() const
{
    return m_iSize;
}

void Cell::addVertex(Cell::Position *point)
{
    m_lstVertex.append(point);
}

void Cell::cleanVertex()
{
    for(int index=0; index < m_lstVertex.count(); ++index) {
        if(NULL != m_lstVertex.front())
            free(m_lstVertex.front());

        m_lstVertex.removeFirst();
    }
}

void Cell::updateVertex()
{
    switch(m_eShape) {
    case EMSHAPE_RECTANGLE:
        cleanVertex();
        addVertex(new Position(x, y));
        addVertex(new Position(x, y + m_iSize));
        addVertex(new Position(x + m_iSize, y + m_iSize));
        addVertex(new Position(x + m_iSize, y));
        break;
    default:
        break;
    }
}

QSGNode *Cell::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGGeometryNode *node = 0;
    QSGGeometry *geometry = 0;

    if (!oldNode) {
        node = new QSGGeometryNode;
        geometry = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(), 4);
        geometry->setLineWidth(2);
        geometry->setDrawingMode(GL_LINE_LOOP);
        node->setGeometry(geometry);
        node->setFlag(QSGNode::OwnsGeometry);
        QSGFlatColorMaterial *material = new QSGFlatColorMaterial;
        material->setColor(QColor(255, 0, 0));
        node->setMaterial(material);
        node->setFlag(QSGNode::OwnsMaterial);
    } else {
        node = static_cast<QSGGeometryNode *>(oldNode);
        geometry = node->geometry();
        geometry->allocate(m_segmentCount);
    }

    QSGGeometry::Point2D *vertices = geometry->vertexDataAsPoint2D();
    for (int i = 0; i < m_lstVertex.count(); ++i) {
        vertices[i].set(m_lstVertex[i].X(), m_lstVertex[i].Y());
    }
    node->markDirty(QSGNode::DirtyGeometry);

    return node;
}

