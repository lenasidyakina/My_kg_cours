#include "Cylinder.h"


#include <QPainter>

void Cylinder::Draw()
{

}

QString Cylinder::getName()
{
    return "Цилиндр " + QString("%1px").arg(this->BaseRadius) + QString("%1px").arg(this->Height);
}

int Cylinder::GetType() { return FIGURE_CYLINDER; }

