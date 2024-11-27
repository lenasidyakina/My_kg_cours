
#include "Cube.h"


#include <QPainter>

void Cube::Draw()
{

}


QString Cube::getName()
{
    return "Куб " + QString("%1px").arg(this->EdgeLength);
}

int Cube::GetType() { return FIGURE_CUBE; }



