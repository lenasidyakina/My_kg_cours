
#ifndef CURS_CUBE_H
#define CURS_CUBE_H

#include <QMetaType>
#include <qpainter.h>

#include "Figure.h"


class Cube : public Figure {
public:
    Cube(QString color, int edgeLength) : Figure(color) {
        this->EdgeLength = edgeLength;
        x = 0; y = 0; z = 0;
    }
    int EdgeLength;
    virtual void Draw() override;
    virtual QString getName() override;
    virtual int GetType() override;

    void Draw(QPainter *painter, Cube *cube);

};

Q_DECLARE_METATYPE( Cube )

#endif //CURS_CUBE_H
