#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <math.h>
#include "SnakeGlobal.h"

Cell::Cell(QQuickItem *parent)
    : QQuickItem(parent)
    , m_pShape(NULL)
    , m_eShape(BaseShape::EMSHAPE_Invalid)
{
    setFlag(ItemHasContents, true);
}

Cell::Cell(BaseShape::ENUM_ESHAPE shape, QQuickItem *parent)
    : QQuickItem(parent)
    , m_pShape(NULL)
    , m_eShape(BaseShape::EMSHAPE_Invalid)
{
    setFlag(ItemHasContents, true);
    setShape(shape);
}

Cell::~Cell()
{

}

void Cell::setShape(BaseShape::ENUM_ESHAPE shape)
{
    BaseShape::ENUM_ESHAPE shapeBak = m_eShape;
    m_eShape = shape;
    switch(shape) {
    case BaseShape::EMSHAPE_Rectangle:
        m_pShape = new RectangleShape(this);
        static_cast<RectangleShape *>(m_pShape)->setBoundColor(Qt::red);
        static_cast<RectangleShape *>(m_pShape)->setContainColor(Qt::green);
        break;
    case BaseShape::EMSHAPE_RoundedRectangle:
        m_pShape = new RoundedRectangleShape(this);
        break;
    case BaseShape::EMSHAPE_Circle:
        m_pShape = new CircleShape(this);
        static_cast<CircleShape *>(m_pShape)->setCirclePoint(400);
        break;
    default:
        return;
    }  
    emit notifyShapeChanged(shape, shapeBak);
}

void Cell::updateAnchios(const QRectF& rect)
{
    m_pShape->setAnchios(rect);


    this->setPosition(QPointF(rect.x(), rect.y()));

    this->setWidth(rect.width());
    this->setHeight(rect.height());
}

void Cell::setBoundSize(qreal size)
{   
    qreal sizeBak = m_pShape->getBoundSize();
    m_pShape->setBoundSize(size);

    emit notifyBoundSizeChanged(m_pShape->getBoundSize(), sizeBak);
}

qreal Cell::boundSize() const
{
    return m_pShape->getBoundSize();
}

QSGNode *Cell::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGNode *backGround = NULL;

    qDebug() << __FUNCTION__;

    m_pShape->updateVertex();

    if (!oldNode) {
        delete oldNode;
    }

    backGround  = new QSGNode;

    QList <QSGGeometry *> lstGeometry = m_pShape->getGeometryList();

    for(int index=0; index < lstGeometry.count(); ++index) {
        QSGGeometryNode *tempNode = new QSGGeometryNode;

        tempNode->setGeometry(lstGeometry[index]);
        tempNode->setFlag(QSGNode::OwnsGeometry);

        tempNode->setMaterial(m_pShape->getGeometryMaterial(lstGeometry[index]));
        tempNode->setFlag(QSGNode::OwnsMaterial);

        QSGGeometry::Point2D*   verticesContain = lstGeometry[index]->vertexDataAsPoint2D();
        QList<QPointF *>        lstVertex = m_pShape->getGeometryVertex(lstGeometry[index]);
        for (int point = 0; point < lstVertex.count(); ++point) {
            verticesContain[point].set(lstVertex[point]->x(), lstVertex[point]->y());
        }

        backGround->appendChildNode(tempNode);
    }
    backGround->markDirty(QSGNode::DirtyGeometry);
    return backGround;
}
