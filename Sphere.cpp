//
// Created by Ivan Sidyakin on 08.11.2024.
//

#include "Sphere.h"


#include <QPainter>

#include "CurTypes.h"

void Sphere::Draw()
{

}


QString Sphere::getName()
{
    return "Сфера " + QString("%1px").arg(this->RadiusLength);
}

int Sphere::GetType() { return FIGURE_SPHERE; }



