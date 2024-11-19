//
// Created by sidya on 19.11.2024.
//

#ifndef CYLINDER_H
#define CYLINDER_H
#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"


class Cylinder : public Figure {
public:
    Cylinder(QString color, int baseRadius, int height) : Figure(color) {
        this->BaseRadius = baseRadius;
        this->Height = height;
        x = 0; y = 0; z = 0;
    }
    int BaseRadius;
    int Height;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;

    void Draw(QPainter *painter, Cylinder *cylinder);

};

Q_DECLARE_METATYPE( Cylinder )
#endif //CYLINDER_H
