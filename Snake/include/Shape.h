#ifndef __SHAPEH__
#define __SHAPEH__

#include <QObject>
#include <QRectF>
#include <QQuickItem>
#include <QSGFlatColorMaterial>

#define PI 3.141592654

class QSGGeometry;
class BaseShape : public QQuickItem
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

        tagGeometryData() {
            m_pMaterial = NULL;
            m_lstVertex.clear();
        }
    };

public:
    BaseShape(QQuickItem *parent = NULL);
    BaseShape(const QRectF& rect, QQuickItem *parent = NULL);
    virtual ~BaseShape() {}

    inline void             setAnchios(const QRectF& rect)          { m_anchios = rect; }
    inline QRectF           getAnchios()                            { return m_anchios; }


    inline void             setBoundSize(qreal size)                { m_fBoundSize = size; }
    inline int              getBoundSize()                          { return m_fBoundSize; }

    void                    addGeometry(QSGGeometry *geometry)      { m_lstGeometry.append(geometry); }
    QList <QSGGeometry *>   getGeometryList()                       { return m_lstGeometry; }

    void                    updateGeometryMaterial(QSGGeometry *key, QSGFlatColorMaterial* material);

    void                    addGeometryVertex(QSGGeometry *key, QPointF *point);
    void                    cleanGeometryVertex(QSGGeometry *key);

    QSGFlatColorMaterial*   getGeometryMaterial(QSGGeometry *key);
    QList<QPointF *>        getGeometryVertex(QSGGeometry *key);

    virtual void            updateVertex() = 0;

public:
    QRectF                                          m_anchios;
    qreal                                           m_fBoundSize;

    QList <QSGGeometry *>                           m_lstGeometry;
    QMap  <QSGGeometry *, tagGeometryData*>         m_lstGeometryData;
};

class RectangleShape : public BaseShape
{
    Q_OBJECT
public:
    RectangleShape(QQuickItem *parent = NULL);
    virtual ~RectangleShape() {}

    void                        updateVertex();

    void                        setContainColor(QColor color);
    QSGFlatColorMaterial*       getContainMaterial();


    void                        setBoundColor(QColor color);
    QSGFlatColorMaterial*       getBoundMaterial();

private:
    QSGGeometry*            m_pGeometryContain;
    QSGGeometry*            m_pGeometryBound;
};

class RoundedRectangleShape : public BaseShape
{
    Q_OBJECT
public:
    RoundedRectangleShape(QQuickItem *parent = NULL);
    virtual ~RoundedRectangleShape() {}

    void                        updateVertex() {}
};

class CircleShape : public BaseShape
{
    Q_OBJECT
public:
    CircleShape(QQuickItem *parent = NULL);
    virtual ~CircleShape() {}

    void                    setCirclePoint(int point)   { m_iCirclePoint = point; }
    int                     getCirclePoint()            { return m_iCirclePoint; }

    void updateVertex();

private:
    int                     m_iCirclePoint;

    QSGGeometry*            m_pGeometryContain;
    QSGGeometry*            m_pGeometryBound;
};

#endif // SHAPE_H
