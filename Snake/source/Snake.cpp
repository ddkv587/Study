#include <QQuickItem>
#include <QSGNode>
#include <QSGSimpleRectNode>
#include <assert.h>
#include "SnakeGlobal.h"

Snake::Snake()
    : m_color(Qt::white)
{
    setFlag(ItemHasContents, true);

//    addCell(QRectF(0, 0, DEF_CELL_SIZE, DEF_CELL_SIZE), BaseShape::EMSHAPE_Rectangle);
//    addCell(QRectF(DEF_CELL_SIZE, 0, DEF_CELL_SIZE, DEF_CELL_SIZE), BaseShape::EMSHAPE_Circle);
//    addCell(QRectF(DEF_CELL_SIZE * 2, 0, DEF_CELL_SIZE, DEF_CELL_SIZE), BaseShape::EMSHAPE_Circle);
    addCell(QRectF(DEF_CELL_SIZE * 3, 0, DEF_CELL_SIZE, DEF_CELL_SIZE), BaseShape::EMSHAPE_Circle);
}

Snake::~Snake()
{

}

void Snake::addCell(const Cell& cell)
{
//    Cell *cellTemp = new Cell(cell);
//    cellTemp->setParent(this);
//    m_lstPCell.insert(0, cellTemp);
}

void Snake::addCell(QRectF rect, BaseShape::ENUM_ESHAPE shape)
{
    Cell *cell = new Cell(shape, this);
    cell->updateAnchios(rect);
    qDebug() << rect.x() << "," << rect.y();
    cell->setBoundSize(2);

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
