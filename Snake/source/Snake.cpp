#include <QQuickItem>
#include <QSGNode>
#include <QSGSimpleRectNode>
#include <assert.h>
#include "SnakeGlobal.h"


Snake::Snake()
    : m_color(Qt::white)
{
    setFlag(ItemHasContents, true);

    addCell(Cell::Position(0, 0), Cell::EMSHAPE_RECTANGLE, DEF_CELL_SIZE);
    addCell(Cell::Position(DEF_CELL_SIZE, 0), Cell::EMSHAPE_RECTANGLE, DEF_CELL_SIZE);
    addCell(Cell::Position(DEF_CELL_SIZE*2, 0), Cell::EMSHAPE_RECTANGLE, DEF_CELL_SIZE);
    addCell(Cell::Position(DEF_CELL_SIZE*3, 0), Cell::EMSHAPE_RECTANGLE, DEF_CELL_SIZE);
}

Snake::~Snake()
{

}

void Snake::addCell(const Cell cell)
{
    Cell *cellTemp = new Cell(cell);
    cellTemp->setParent(this);
    m_lstPCell.insert(0, cellTemp);
}

void Snake::addCell(Cell::Position posi, Cell::ENUM_ESHAPE shape, int size)
{
    Cell *cell = new Cell(this);

    cell->setX(posi.X());
    cell->setY(posi.Y());
    cell->setShape(shape);
    cell->setSize(size);
    cell->setBoundSize(2);

    qDebug() << "<" << posi.X() << "," << posi.Y() << ">" <<endl;

    m_lstPCell.insert(0, cell);
}

Cell *Snake::delCell()
{
    assert(m_lstPCell.count() > 0);

    Cell *cell = m_lstPCell.last();
    m_lstPCell.removeLast();
    return cell;
}

void Snake::setColor(QColor color)
{
    QColor colorBak = m_color;
    m_color = color;
    emit notifyColorChanged(m_color, colorBak);
}

QColor Snake::color()
{
    return m_color;
}

QSGNode *Snake::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    Q_UNUSED(data)
    QSGSimpleRectNode *n = static_cast<QSGSimpleRectNode *>(node);
    if (!n) {
        n = new QSGSimpleRectNode();
    }
    n->setColor(m_color);
    n->setRect(boundingRect());

    for(int i=0; i<m_lstPCell.count(); ++i)
    {
        m_lstPCell[i]->update();
    }
    return n;
}
