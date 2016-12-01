#include <QQuickItem>
#include <QSGNode>
#include <assert.h>
#include "SnakeGlobal.h"


Snake::Snake()
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
    m_lstPCell.insert(0, cellTemp);
}

void Snake::addCell(Cell::Position posi, Cell::ENUM_ESHAPE shape, int size)
{
    Cell *cell = new Cell;

    cell->setX(posi.X());
    cell->setY(posi.Y());
    cell->setShape(shape);
    cell->setSize(size);

    m_lstPCell.insert(0, cell);
}

Cell &Snake::delCell()
{
    assert(m_lstPCell.count() > 0);

    Cell cell(*m_lstPCell.last());
    m_lstPCell.removeLast();
    return cell;
}

QSGNode *Snake::updatePaintNode(QSGNode *node, QQuickItem::UpdatePaintNodeData *data)
{
    for(int i=0; i<m_lstPCell.count(); ++i)
    {
        m_lstPCell[i]->update();
    }
}
