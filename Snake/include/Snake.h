#ifndef __SNAKEH__
#define __SNAKEH__

#include <QQuickItem>

class Cell;
class Snake : public QQuickItem
{
    Q_OBJECT

public:
    Snake();
    virtual ~Snake();

    void addCell(const Cell& cell);
    Cell& delCell();

    void setParent(QQuickItem* parent);
    void updatePaint();


private:
    QList<Cell> m_lstCell;
};

#endif
