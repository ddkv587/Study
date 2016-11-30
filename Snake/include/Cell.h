#ifndef __CELLH__
#define __CELLH__

#include <QQuickItem>

class Cell : public QQuickItem
{
    Q_OBJECT
public:
    enum ENUM_SHAPE {
        EMSHAPE_INVALID 	= -1,
        EMSHAPE_RECTANGLE,
        EMSHAPE_CIRCLE,
        EMSHAPE_MAX
    };

public:
    Cell();
    virtual ~Cell();

    void setShape(ENUM_SHAPE shape);
    ENUM_SHAPE shape();

signals:
    void notifyShapeChanged(ENUM_SHAPE newShape, ENUM_SHAPE oldShape = EMSHAPE_INVALID);

public slots:

private:

private:
    ENUM_SHAPE m_shape;
};
#endif
