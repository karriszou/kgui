#include "Rect.h"


Rect::Rect()
    :x(0),
     y(0),
     w(0),
     h(0)
{
}

Rect::Rect(float x, float y, float w, float h)
    :x(x),
     y(y),
     w(w),
     h(h)
{

}

void Rect::set(float x, float y, float w, float h)
{
    this->x = x;
    this->y = y;
    this->w = w;
    this->h = h;
}

bool Rect::contain(float x, float y)
{
    return x > this->x && x < this->x + this->w && y > this->y && y < this->y + this->h;
}

void Rect::scale(float wfactor, float hfactor)
{
    this->w *= wfactor; this->h *= hfactor;
}

void Rect::scaleFromCenter(float wfactor, float hfactor)
{
    float oldw = this->w, oldh = this->h;
    this->w *= wfactor; this->h *= hfactor;
    this->x += (oldw - this->w) / 2;
    this->y += (oldh - this->h) / 2;
}
