#include "SnakeGlobal.h"

namespace SNAKE
{
    Cell::Cell()
    {

    }

    Cell::~Cell()
    {

    }

    void Cell::setShape(Cell::ENUM_SHAPE shape)
    {
        ENUM_SHAPE shapeBak = m_shape;
        m_shape = shape;
        emit notifyShapeChanged(m_shape, shapeBak);
    }

    Cell::ENUM_SHAPE Cell::shape()
    {
        return m_shape;
    }
}
