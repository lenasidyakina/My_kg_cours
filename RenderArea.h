//
// Created by Ivan Sidyakin on 10.11.2024.
//

#ifndef CURS_RENDERAREA_H
#define CURS_RENDERAREA_H


#include <QWidget>

#include "Cube.h"
#include "Figure.h"


class RenderArea : public QWidget {
    Q_OBJECT
public:
    explicit RenderArea(QWidget *parent);
    void invalidate();
protected:
    QBrush bkgndBrush;
    void paintEvent(QPaintEvent *event) override;


    const QList<Figure*> & FigureList();
};


#endif //CURS_RENDERAREA_H
