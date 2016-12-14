#ifndef __SHAPEH__
#define __SHAPEH__

#include <QObject>

class QRectF;
class QSGGeometry;
class QSGFlatColorMaterial;
class BaseShape : public QObject
{
    Q_OBJECT
public:
    enum ENUM_ESHAPE {
        EMSHAPE_Invalid 	= -1,
        EMSHAPE_Rectangle,
        EMSHAPE_RoundedRectangle,
        EMSHAPE_Circle,
        EMSHAPE_MAX
    };

    struct tagGeometryData {
        QSGFlatColorMaterial*   m_pMaterial;
        QList<QPointF *>        m_lstVertex;
    };

public:
    BaseShape(const QRectF& rect);
    virtual ~BaseShape() {}

    void                    addGeometry(QSGGeometry *geometry)      { m_lstGeometry.append(geometry); }
    QList <QSGGeometry *>   getGeometry()                           { return m_lstGeometry; }

    void                    updateGeometryMaterial(QSGGeometry *geometry);

    void                    addGeometryVertex(QSGGeometry *key, QPointF *point);
    void                    cleanGeometryVertex(QSGGeometry *key);
    void                    updateGeometryVertex(QSGGeometry *) = 0;

    QSGFlatColorMaterial*   getGeometryMaterial(QSGGeometry *key);
    QList<QPointF *>        getGeometryVertex(QSGGeometry *key);

private:
    QRectF                                          m_anchios;

    QList <QSGGeometry *>                           m_lstGeometry;
    QMap  <QSGGeometry *, tagGeometryData>          m_lstGeometryData;
};

class RectangleShape : public BaseShape
{
    Q_OBJECT
public:
    RectangleShape();
    virtual ~RectangleShape() {}

    void            updateVertex(QSGGeometry *key);

    inline void     setContainColor(QColor color)   { m_containColor = color; }
    inline QColor   getContainColor()               { return m_containColor; }

    inline void     setBoundColor(QColor color)     { m_boundColor = color; }
    inline QColor   getBoundColor()                 { return m_boundColor; }

private:

    QSGGeometry*    m_pGeometryContain;
    QSGGeometry*    m_pGeometryBound;

    QColor          m_containColor;
    QColor          m_boundColor;
};

class RoundedRectangleShape : public BaseShape
{

};

class CircleShape : public BaseShape
{

};

#endif // SHAPE_H
