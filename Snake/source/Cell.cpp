#include <QSGNode>
#include <QSGFlatColorMaterial>
#include <math.h>
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

    m_pGeoContain = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertex.count());
    m_pGeoBound   = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertexBound.count());
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

    m_pGeoContain = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertex.count());
    m_pGeoBound   = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  m_lstVertexBound.count());
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

        m_pGeoContain->setDrawingMode(GL_TRIANGLE_FAN);
        m_pGeoContain->allocate(m_lstVertex.count());

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

            m_pGeoBound->setDrawingMode(GL_LINES);
            m_pGeoBound->allocate(m_lstVertexBound.count());
            m_pGeoBound->setLineWidth(m_fBoundSize);
        }
        break;

    case EMSHAPE_RoundedRectangle:
        cleanVertex();

        break;
    case EMSHAPE_Circle:
        cleanVertex();
        addVertex(new Position(m_fSize / 2, m_fSize / 2));
        float r = m_fSize / 2 - m_fBoundSize;
        for (int index = 0; index <= DEF_CIRCLE_COUNT; ++index) {
            addVertex(new Position(m_fSize / 2 + r * cos(2.0 * PI / DEF_CIRCLE_COUNT * index), m_fSize / 2 + r * sin(2.0 * PI / DEF_CIRCLE_COUNT * index)));
        }
        m_pGeoContain->setDrawingMode(GL_TRIANGLE_FAN);
        m_pGeoContain->allocate(m_lstVertex.count());

        if (m_fBoundSize > 0) {
            for (int index = 0; index <= DEF_CIRCLE_COUNT; ++index) {
                addBoundVertex(new Position(m_fSize / 2 + (r + m_fBoundSize / 2) * cos(2 * PI / DEF_CIRCLE_COUNT * index), m_fSize / 2 + (r + m_fBoundSize/2) * sin(2 * PI / DEF_CIRCLE_COUNT * index)));
            }
            m_pGeoBound->setDrawingMode(GL_LINE_LOOP);
            m_pGeoBound->allocate(m_lstVertexBound.count());
            m_pGeoBound->setLineWidth(m_fBoundSize);
        }
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

    QSGGeometryNode *contain;
    QSGGeometryNode *bound;

    qDebug() << __FUNCTION__;

    updateVertex();

    if (!oldNode) {
        backGround  = new QSGNode;

        contain     = new QSGGeometryNode;
        bound       = new QSGGeometryNode;

        contain->setGeometry(m_pGeoContain);
        contain->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *materialContain = new QSGFlatColorMaterial;
        materialContain->setColor(QColor(255, 0, 0));
        contain->setMaterial(materialContain);
        contain->setFlag(QSGNode::OwnsMaterial);



        bound->setGeometry(m_pGeoBound);
        bound->setFlag(QSGNode::OwnsGeometry);

        QSGFlatColorMaterial *materialBound = new QSGFlatColorMaterial;
        materialBound->setColor(QColor(0, 0, 255));
        bound->setMaterial(materialBound);
        bound->setFlag(QSGNode::OwnsMaterial);


        backGround->appendChildNode(contain);
        backGround->appendChildNode(bound);
    } else {
        backGround = oldNode;
    }

    QSGGeometry::Point2D *verticesContain = m_pGeoContain->vertexDataAsPoint2D();
    qDebug()<<"=================== begin";
    for (int i = 0; i < m_lstVertex.count(); ++i) {
        qDebug() << "<" << m_lstVertex[i]->X() << "," << m_lstVertex[i]->Y() << ">";
        verticesContain[i].set(m_lstVertex[i]->X(), m_lstVertex[i]->Y());
    }

    QSGGeometry::Point2D *verticesBound = m_pGeoBound->vertexDataAsPoint2D();
    qDebug()<<"=================== begin";
    for (int i = 0; i < m_lstVertexBound.count(); ++i) {
        qDebug() << "<" << m_lstVertexBound[i]->X() << "," << m_lstVertexBound[i]->Y() << ">";
        verticesBound[i].set(m_lstVertexBound[i]->X(), m_lstVertexBound[i]->Y());
    }
    qDebug()<<"=================== end";

    backGround->markDirty(QSGNode::DirtyGeometry);
    return backGround;
}
