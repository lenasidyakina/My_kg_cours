//
// Created by Ivan Sidyakin on 08.11.2024.
//

#ifndef CURS_FIGURE_H
#define CURS_FIGURE_H

#include <QString>
#include <QVector3D>
#include "CurTypes.h"


class Figure {

public:
    int color;
    int x, y, z;

    Figure(QString color) {
        this->color = ColorToInt(color);
    }

    virtual void Draw() {};
    virtual QString getName() { return ""; };
    virtual int GetType() { return FIGURE_UNDEFINED; }
    static uint ColorToInt(const QString color)
    {
        bool ok;
        uint c =  color.mid(1).toUInt(&ok, 16);
        if (!ok)
            c = QString(DEFAULT_FIGURE_COLOR).mid(1).toUInt(&ok, 16);
        return c;
    }

};


#endif //CURS_FIGURE_H
