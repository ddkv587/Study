#include <QSGNode>
#include <QSGFlatColorMaterial>
#include "SnakeGlobal.h"

Cell::Cell(QQuickItem *parent)
    : QQuickItem(parent)
    , m_fSize(0)
    , m_fBoundSize(0)
    , m_eShape(EMSHAPE_Invalid)
{
    setFlag(ItemHasContents, true);
    setX(0);
    setY(0);
}

Cell::Cell(const Cell &cell, QQuickItem *parent)
    : QQuickItem(parent)
{
    setFlag(ItemHasContents, true);
    setX(cell.x());
    setY(cell.y());

    m_fSize = cell.size();
    m_fBoundSize = cell.boundSize();
    m_eShape = cell.shape();   
}

Cell::~Cell()
{

}

void Cell::setShape(Cell::ENUM_ESHAPE shape)
{
    ENUM_ESHAPE shapeBak = m_eShape;
    m_eShape = shape;

    emit notifyShapeChanged(m_eShape, shapeBak);
}

Cell::ENUM_ESHAPE Cell::shape() const
{
    return m_eShape;
}

void Cell::setSize(int size)
{
    int sizeBak = m_fSize;
    m_fSize = size;

    emit notifySizeChanged(m_fSize, sizeBak);
}

int Cell::size() const
{
    return m_fSize;
}

void Cell::setBoundSize(int size)
{
    int sizeBak = m_fBoundSize;
    m_fBoundSize = size;

    emit notifyBoundSizeChanged(m_fBoundSize, sizeBak);
}

int Cell::boundSize() const
{
    return m_fBoundSize;
}

void Cell::addVertex(Cell::Position *point)
{
    qDebug() << __FUNCTION__ << "<" << point->X() << "," << point->Y() << ">";
    m_lstVertex.append(point);
}

void Cell::cleanVertex()
{
    int count = m_lstVertex.count();
    for(int index=0; index < count; ++index) {
        if(NULL != m_lstVertex.front())
            free(m_lstVertex.front());

        m_lstVertex.removeFirst();
    }
}

void Cell::updateVertex()
{
    switch(m_eShape) {
    case EMSHAPE_Rectangle:
        cleanVertex();
        addVertex(new Position(m_fBoundSize, m_fBoundSize));
        addVertex(new Position(m_fBoundSize, m_fSize - m_fBoundSize));
        addVertex(new Position(m_fSize - m_fBoundSize, m_fSize - m_fBoundSize));
        addVertex(new Position(m_fSize - m_fBoundSize, m_fBoundSize));

        if(m_fBoundSize > 0) {
            cleanBoundVertex();
            addBoundVertex(new Position(0, m_fBoundSize / 2));
            addBoundVertex(new Position(m_fSize, m_fBoundSize / 2));

            addBoundVertex(new Position(0, m_fSize - m_fBoundSize / 2));
            addBoundVertex(new Position(m_fSize, m_fSize - m_fBoundSize / 2));

            addBoundVertex(new Position(m_fBoundSize / 2, m_fBoundSize / 2));
            addBoundVertex(new Position(m_fBoundSize / 2, m_fSize - m_fBoundSize / 2));

            addBoundVertex(new Position(m_fSize - m_fBoundSize / 2, m_fBoundSize / 2));
            addBoundVertex(new Position(m_fSize - m_fBoundSize / 2, m_fSize - m_fBoundSize / 2));
        }
        break;

    case EMSHAPE_RoundedRectangle:
        cleanVertex();


        break;
    case EMSHAPE_Circle:
        cleanVertex();

        break;
    default:
        break;
    }
}

void Cell::addBoundVertex(Cell::Position *point)
{
    qDebug() << __FUNCTION__ << "<" << point->X() << "," << point->Y() << ">";
    m_lstVertexBound.append(point);
}

void Cell::cleanBoundVertex()
{
    int count = m_lstVertexBound.count();
    for(int index=0; index < count; ++index) {
        if(NULL != m_lstVertexBound.front())
            free(m_lstVertexBound.front());

        m_lstVertexBound.removeFirst();
    }
}

QSGNode *Cell::updatePaintNode(QSGNode *oldNode, QQuickItem::UpdatePaintNodeData *)
{
    QSGNode *backGround = NULL;

    QSGGeometry *geometryContain;
    QSGGeometry *geometryBound;

    qDebug() << __FUNCTION__;

    updateVertex();

    if (!oldNode) {
        backGround = new QSGNode;

        QSGGeometryNode *contain = new QSGGeometryNode;
        QSGGeometryNode *bound   = new QSGGeometryNode;

        geometryContain = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertex.count());
        geometryBound   = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertexBound.count());



        geometryContain->setDrawingMode(GL_TRIANGLE_FAN);
        geometryContain->allocate(m_lstVertex.count());
        contain->setGeometry(geometryContain);
        contain->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *materialContain = new QSGFlatColorMaterial;
        materialContain->setColor(QColor(255, 0, 0));
        contain->setMaterial(materialContain);
        contain->setFlag(QSGNode::OwnsMaterial);



        geometryBound->setDrawingMode(GL_LINES);
        geometryBound->allocate(m_lstVertexBound.count());
        geometryBound->setLineWidth(m_fBoundSize);
        bound->setGeometry(geometryBound);
        bound->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *materialBound = new QSGFlatColorMaterial;
        materialBound->setColor(QColor(0, 0, 255));
        bound->setMaterial(materialBound);
        bound->setFlag(QSGNode::OwnsMaterial);


        backGround->appendChildNode(contain);
        backGround->appendChildNode(bound);
    } else {
        backGround = oldNode;
        geometryContain = (static_cast<QSGGeometryNode *>(backGround->childAtIndex(0)))->geometry();
        geometryBound   = (static_cast<QSGGeometryNode *>(backGround->childAtIndex(1)))->geometry();
    }

    QSGGeometry::Point2D *verticesContain = geometryContain->vertexDataAsPoint2D();
    qDebug()<<"=================== begin";
    for (int i = 0; i < m_lstVertex.count(); ++i) {
        qDebug() << "<" << m_lstVertex[i]->X() << "," << m_lstVertex[i]->Y() << ">";
        verticesContain[i].set(m_lstVertex[i]->X(), m_lstVertex[i]->Y());
    }

    QSGGeometry::Point2D *verticesBound = geometryBound->vertexDataAsPoint2D();
    qDebug()<<"=================== begin";
    for (int i = 0; i < m_lstVertexBound.count(); ++i) {
        qDebug() << "<" << m_lstVertexBound[i]->X() << "," << m_lstVertexBound[i]->Y() << ">";
        verticesBound[i].set(m_lstVertexBound[i]->X(), m_lstVertexBound[i]->Y());
    }
    qDebug()<<"=================== end";

    backGround->markDirty(QSGNode::DirtyGeometry);
    return backGround;
}
