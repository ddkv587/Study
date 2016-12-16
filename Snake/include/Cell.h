#ifndef __CELLH__
#define __CELLH__

#include <QQuickItem>
#include "Shape.h"

class Cell : public QQuickItem
{
    Q_OBJECT
public:

public:
    Cell(QQuickItem *parent = NULL);
    Cell(BaseShape::ENUM_ESHAPE shape, QQuickItem *parent = NULL);
    virtual ~Cell();

    QSGNode             *updatePaintNode(QSGNode *oldNode, UpdatePaintNodeData *);

    void                setBoundSize(qreal size);
    qreal               boundSize() const;

    void                            setShape(BaseShape::ENUM_ESHAPE shape);
    inline BaseShape::ENUM_ESHAPE   getCurrentShape()                           { return m_eShape; }

    void                updateAnchios(const QRectF &rect);

signals:
    void                notifyShapeChanged(BaseShape::ENUM_ESHAPE newShape, BaseShape::ENUM_ESHAPE oldShape);
    void                notifyBoundSizeChanged(int newSize, int oldSize);

public slots:

private:

private:
    BaseShape *m_pShape;
    BaseShape::ENUM_ESHAPE m_eShape;
};
#endif
