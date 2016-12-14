#ifndef __CELLH__
#define __CELLH__

#include <QQuickItem>

#define DEF_CIRCLE_COUNT 100
#define PI 3.141592654


class QSGGeometry;

class Cell : public QQuickItem
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

    struct Position {

        inline float X()           { return x; }
        inline float Y()           { return y; }
        inline void setX(float _x) { x = _x; }
        inline void setY(float _y) { y = _y; }

        inline bool operator ==(Position temp) {
            return (temp.X() == x && temp.Y() == y) ? true : false;
        }

        Position(float _x,float _y) {x = _x; y = _y;}

    private:
        float x;
        float y;
    };

public:
    Cell(QQuickItem *parent = NULL);
    Cell(const Cell &cell, QQuickItem *parent = NULL);
    virtual ~Cell();

    void                setShape(ENUM_ESHAPE shape);
    ENUM_ESHAPE         shape() const;

    void                setSize(int size);
    int                 size() const;

    void                setBoundSize(int size);
    int                 boundSize() const;

    void                addVertex(Position *point);
//    Position&           delVertex(const Position point) { Q_UNUSED(point) }
    void                cleanVertex();
    void                updateVertex();

    void                addBoundVertex(Cell::Position *point);
    void                cleanBoundVertex();
    void                updateBoundVertex();

    QSGNode             *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);


signals:
    void                notifyShapeChanged(ENUM_ESHAPE newShape, ENUM_ESHAPE oldShape);
    void                notifySizeChanged(int newSize, int oldSize);
    void                notifyBoundSizeChanged(int newSize, int oldSize);

public slots:

private:

private:
    float m_fSize;
    float m_fBoundSize;
    QList<Position *> m_lstVertex;
    QList<Position *> m_lstVertexBound;
    ENUM_ESHAPE m_eShape;
    QSGGeometry *m_pGeoContain;
    QSGGeometry *m_pGeoBound;
};
#endif
