//
// Created by sidya on 19.11.2024.
//

#ifndef SPHERE_H
#define SPHERE_H


#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"


class Sphere : public Figure {
public:
    Sphere(QString color, int radiusLength) : Figure(color) {
        this->RadiusLength = radiusLength;
        x = 0; y = 0; z = 0;
    }
    int RadiusLength;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;
    int slices = DEFAULT_SLICES; // Количество секторов по горизонтали
    int stacks = DEFAULT_STACKS; // Количество секторов по вертикали

    void Draw(QPainter *painter, Sphere *cube);

    void Draw_base_sphere(QPainter *painter, Sphere *sphere);
};

Q_DECLARE_METATYPE( Sphere )

#endif //SPHERE_H
