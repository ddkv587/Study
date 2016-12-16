#ifndef __SNAKEH__
#define __SNAKEH__

#include <QQuickItem>
#include "Cell.h"

#define DEF_CELL_SIZE 20

class Snake : public QQuickItem
{
    Q_OBJECT

public:
    Q_PROPERTY(QColor color READ color WRITE setColor NOTIFY notifyColorChanged)

    Snake();
    virtual ~Snake();

    void        addCell(const Cell &cell);
    void        addCell(QRectF rect, BaseShape::ENUM_ESHAPE shape);
    Cell        *delCell();

    void        setParent(QQuickItem* parent);
    void        updatePaint();

    void        setColor(QColor color);
    QColor      color();

    QSGNode     *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data);

signals:
    void        notifyColorChanged(QColor newColor, QColor oldColor);

private:
    QColor m_color;
    QList<Cell *> m_lstPCell;
};

#endif
