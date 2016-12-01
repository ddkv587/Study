#ifndef __SNAKEH__
#define __SNAKEH__

#include <QQuickItem>

#define DEF_CELL_SIZE 10

class Cell;
class Snake : public QQuickItem
{
    Q_OBJECT

public:

    Snake();
    virtual ~Snake();

    void addCell(const Cell cell);
    void addCell(Cell::Position posi, Cell::ENUM_ESHAPE shape, int size);
    Cell& delCell();

    void setParent(QQuickItem* parent);
    void updatePaint();

    QSGNode *updatePaintNode(QSGNode *node, UpdatePaintNodeData *data);


private:
    QList<Cell *> m_lstPCell;
};

#endif
