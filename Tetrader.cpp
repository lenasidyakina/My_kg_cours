#include "Tetrader.h"


#include <QPainter>

#include "CurTypes.h"

void Tetrader::Draw()
{

}


QString Tetrader::getName()
{
    return "Тетрайдер " + QString("%1px").arg(this->Height);
}

int Tetrader::GetType() { return FIGURE_PYRAMID; }