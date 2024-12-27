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
    Sphere() : Figure(DEFAULT_FIGURE_COLOR) {
        this->RadiusLength = DEFAULT_BASE_SPHERE_RADIUS;
        x = 0; y = 0; z = 0;
    }
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

    void Draw(QPainter *painter, Sphere *globe, Sphere *sphere, std::vector<Poly> &polys);
    void DrawGlobe(QPainter *painter, Sphere *sphere, std::vector<Poly> &polys);

};

Q_DECLARE_METATYPE( Sphere )

#endif //SPHERE_H
