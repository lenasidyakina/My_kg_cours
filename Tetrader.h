
#ifndef TETRADER_H
#define TETRADER_H

#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"
#include "Sphere.h"


class Tetrader : public Figure {
public:
    Tetrader(QString color, int height) : Figure(color) {
        this->Height = height;
        x = 0; y = 0; z = 0;
    }
    int Height;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;
    int slices = DEFAULT_SLICES; // Количество секторов по горизонтали
    int stacks = DEFAULT_STACKS; // Количество секторов по вертикали

    void Draw(QPainter *painter, Tetrader *tetrader);

    void Draw(QPainter *painter, Sphere *base_sphere, Tetrader *tetrahedron, Trajectory *trajectory);
    QVector3D center; // Центр тетраэдра

    QVector3D getPosition() const {
        return center; // Возвращаем центр тетраэдра
    }
    void Draw(QPainter *painter, Sphere *base_sphere, Tetrader *tetrahedron, Trajectory *trajectory, std::vector<Poly> &polys);
};

Q_DECLARE_METATYPE( Tetrader )

#endif //TETRADER_H
