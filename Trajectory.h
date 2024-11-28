//
// Created by sidya on 21.11.2024.
//

#ifndef TRAJECTORY_H
#define TRAJECTORY_H

#include <QMetaType>
#include <qpainter.h>

#include "CurTypes.h"


class Trajectory {
public:
    Trajectory(QString color, int phi, int theta) {
        this->phi = phi;
        this->theta = theta;
        this->color = ColorToInt(color);
    }
    int color = 958;
    int phi = 0;
    int theta = 0;
    static uint ColorToInt(const QString color)
    {
        bool ok;
        uint c =  color.mid(1).toUInt(&ok, 16);
        if (!ok)
            c = QString(DEFAULT_FIGURE_COLOR).mid(1).toUInt(&ok, 16);
        return c;
    }

};

Q_DECLARE_METATYPE( Trajectory )

#endif //TRAJECTORY_H
