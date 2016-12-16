#include <QSGGeometry>
#include <QSGFlatColorMaterial>
#include <math.h>
#include "SnakeGlobal.h"

BaseShape::BaseShape(QQuickItem *parent)
    : QQuickItem(parent)
    , m_anchios()
    , m_fBoundSize(0)
{

}

BaseShape::BaseShape(const QRectF &rect, QQuickItem *parent)
    : QQuickItem(parent)
    , m_anchios(rect)
{

}

void BaseShape::updateGeometryMaterial(QSGGeometry *key, QSGFlatColorMaterial *material)
{
    if(m_lstGeometryData.contains(key)) {
        if(NULL != m_lstGeometryData.value(key)->m_pMaterial) {
            free(m_lstGeometryData.value(key)->m_pMaterial);
        }
        m_lstGeometryData.value(key)->m_pMaterial = material;
    }
}

void BaseShape::addGeometryVertex(QSGGeometry *key, QPointF *point)
{
    if(m_lstGeometryData.contains(key)) {
        tagGeometryData *pData = m_lstGeometryData.value(key);

        QPointF *pointTemp = new QPointF(point->x(), point->y());
        pData->m_lstVertex.append(pointTemp);
    }
}

void BaseShape::cleanGeometryVertex(QSGGeometry *key)
{
    if(m_lstGeometryData.contains(key)) {
        tagGeometryData *pData = m_lstGeometryData.value(key);

        for(int index=0; index < pData->m_lstVertex.count(); ++index) {
            free(pData->m_lstVertex[index]);
            pData->m_lstVertex[index] = NULL;
        }
        pData->m_lstVertex.clear();
    }
}

QSGFlatColorMaterial *BaseShape::getGeometryMaterial(QSGGeometry *key)
{
    if(m_lstGeometryData.contains(key)) {
        tagGeometryData *pData = m_lstGeometryData.value(key);
        return pData->m_pMaterial;
    }

    return NULL;
}

QList<QPointF *> BaseShape::getGeometryVertex(QSGGeometry *key)
{
    if(m_lstGeometryData.contains(key)) {
        tagGeometryData *pData = m_lstGeometryData.value(key);
        return pData->m_lstVertex;
    }
}

RectangleShape::RectangleShape(QQuickItem *parent)
    : BaseShape(parent)
    , m_pGeometryContain(NULL)
    , m_pGeometryBound(NULL)
{
    m_pGeometryContain = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  0);
    m_pGeometryBound   = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  0);

    addGeometry(m_pGeometryContain);
    addGeometry(m_pGeometryBound);

    m_lstGeometryData[m_pGeometryContain] = new tagGeometryData();
    m_lstGeometryData[m_pGeometryBound] = new tagGeometryData();
}

void RectangleShape::updateVertex()
{
    if(m_lstGeometry.contains(m_pGeometryContain)) {
        cleanGeometryVertex(m_pGeometryContain);

        addGeometryVertex(m_pGeometryContain, new QPointF(m_fBoundSize, m_fBoundSize));
        addGeometryVertex(m_pGeometryContain, new QPointF(m_fBoundSize, m_anchios.height() - m_fBoundSize));
        addGeometryVertex(m_pGeometryContain, new QPointF(m_anchios.width() - m_fBoundSize, m_anchios.height() - m_fBoundSize));
        addGeometryVertex(m_pGeometryContain, new QPointF(m_anchios.width() - m_fBoundSize, m_fBoundSize));

        m_pGeometryContain->setDrawingMode(GL_TRIANGLE_FAN);
        m_pGeometryContain->allocate( m_lstGeometryData[m_pGeometryContain]->m_lstVertex.count() );

        if(m_fBoundSize > 0) {
            cleanGeometryVertex(m_pGeometryBound);
            addGeometryVertex(m_pGeometryBound, new QPointF(0, m_fBoundSize / 2));
            addGeometryVertex(m_pGeometryBound, new QPointF(m_anchios.width(), m_fBoundSize / 2));

            addGeometryVertex(m_pGeometryBound, new QPointF(0, m_anchios.height() - m_fBoundSize / 2));
            addGeometryVertex(m_pGeometryBound, new QPointF(m_anchios.width(), m_anchios.height() - m_fBoundSize / 2));

            addGeometryVertex(m_pGeometryBound, new QPointF(m_fBoundSize / 2, m_fBoundSize / 2));
            addGeometryVertex(m_pGeometryBound, new QPointF(m_fBoundSize / 2, m_anchios.height() - m_fBoundSize / 2));

            addGeometryVertex(m_pGeometryBound, new QPointF(m_anchios.width() - m_fBoundSize / 2, m_fBoundSize / 2));
            addGeometryVertex(m_pGeometryBound, new QPointF(m_anchios.width() - m_fBoundSize / 2, m_anchios.height() - m_fBoundSize / 2));

            m_pGeometryBound->setDrawingMode(GL_LINES);
            m_pGeometryBound->allocate( m_lstGeometryData[m_pGeometryBound]->m_lstVertex.count() );
            m_pGeometryBound->setLineWidth(m_fBoundSize);
        }
    }
}

void RectangleShape::setContainColor(QColor color)
{
    if( m_lstGeometryData.contains(m_pGeometryContain) ) {
        QSGFlatColorMaterial *pMaterial;
        if(NULL == (pMaterial = getGeometryMaterial(m_pGeometryContain))) {
            pMaterial = new QSGFlatColorMaterial;
        }
        pMaterial->setColor(color);

        updateGeometryMaterial(m_pGeometryContain, pMaterial);
    }
}

QSGFlatColorMaterial* RectangleShape::getContainMaterial()
{
    if( m_lstGeometryData.contains(m_pGeometryContain) ) {
        return getGeometryMaterial(m_pGeometryContain);
    }
    return NULL;
}

void RectangleShape::setBoundColor(QColor color)
{
    if( m_lstGeometryData.contains(m_pGeometryBound) ) {
        QSGFlatColorMaterial *pMaterial;
        if(NULL == (pMaterial = getGeometryMaterial(m_pGeometryBound))) {
            pMaterial = new QSGFlatColorMaterial;
        }
        pMaterial->setColor(color);
        updateGeometryMaterial(m_pGeometryBound, pMaterial);
    }
}

QSGFlatColorMaterial* RectangleShape::getBoundMaterial()
{
    if( m_lstGeometryData.contains(m_pGeometryBound) ) {
        return getGeometryMaterial(m_pGeometryBound);
    }
    return NULL;
}

RoundedRectangleShape::RoundedRectangleShape(QQuickItem *parent)
    : BaseShape(parent)
{

}

CircleShape::CircleShape(QQuickItem *parent)
    : BaseShape(parent)
    , m_iCirclePoint(200)
    , m_pGeometryContain(NULL)
    , m_pGeometryBound(NULL)
{
    m_pGeometryContain = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  0);
    m_pGeometryBound   = new QSGGeometry(QSGGeometry::defaultAttributes_Point2D(),  0);

    addGeometry(m_pGeometryContain);
    addGeometry(m_pGeometryBound);

    m_lstGeometryData[m_pGeometryContain] = new tagGeometryData();
    m_lstGeometryData[m_pGeometryBound] = new tagGeometryData();
}

void CircleShape::updateVertex()
{
    if(m_lstGeometry.contains(m_pGeometryContain)) {

        cleanGeometryVertex(m_pGeometryContain);
        qreal less = m_anchios.width() < m_anchios.height() ? m_anchios.width() : m_anchios.height();
        qreal r = less / 2 - m_fBoundSize;

        addGeometryVertex(m_pGeometryContain, new QPointF(less / 2, less / 2));
        for(int index=0; index < m_iCirclePoint; ++index) {
            addGeometryVertex(m_pGeometryContain, new QPointF(less / 2 + r * cos(2 * PI / m_iCirclePoint * index) , \
                                                              less / 2 + r * sin(2 * PI / m_iCirclePoint * index)));
        }
        m_pGeometryContain->setDrawingMode(GL_TRIANGLE_FAN);
        qDebug() << m_lstGeometryData[m_pGeometryContain]->m_lstVertex.count();
        m_pGeometryContain->allocate( m_lstGeometryData[m_pGeometryContain]->m_lstVertex.count() );

        if(m_fBoundSize > 0) {
            cleanGeometryVertex(m_pGeometryBound);
            for(int index=0; index < m_iCirclePoint; ++index) {
                addGeometryVertex(m_pGeometryBound, new QPointF(less / 2 + (r + m_fBoundSize / 2) * cos(2 * PI / m_iCirclePoint * index) , \
                                                                  less / 2 + (r + m_fBoundSize / 2) * sin(2 * PI / m_iCirclePoint * index)));
            }

            m_pGeometryBound->setDrawingMode(GL_LINE_LOOP);
            qDebug() << m_lstGeometryData[m_pGeometryBound]->m_lstVertex.count();
            m_pGeometryBound->allocate( m_lstGeometryData[m_pGeometryBound]->m_lstVertex.count() );
            m_pGeometryBound->setLineWidth(m_fBoundSize);
        }
    }
}


