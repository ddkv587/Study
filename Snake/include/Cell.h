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

    ENUM_ESHAPE m_eShape;

};
#endif
