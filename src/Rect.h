#ifndef RECT_H
#define RECT_H


#include <iostream>
#include "Vector.h"


//
// Rect - base on screen coordinate
// Top-left corner is (x, y), bottom-right corner is (x + w, y + h)
//
class Rect
{
public:
    float x, y, w, h;

    Rect();
    Rect(float x, float y, float w, float h);

    void set(float x, float y, float w, float h);

    bool contain(float x, float y);

    void scale(float wfactor, float hfactor);

    void scaleFromCenter(float wfactor, float hfactor);

    inline float left()   { return x; }

    inline float right()  { return x + w; }

    inline float top()    { return y; }

    inline float bottom() { return y + h; }

    inline math::vec2 topLeft()  { return math::vec2(x, y); }

    inline math::vec2 topRight() { return math::vec2(x + w, y); }

    inline math::vec2 botRight() { return math::vec2(x + w, y + h); }
    
    inline math::vec2 botLeft()  { return math::vec2(x, y + h); }

    inline math::vec2 center()   { return math::vec2(x + w / 2, y + h / 2); }

    inline void print() { std::cout << "[ " << x << ", " << y << ", " << w << ", " << h << " ]" << std::endl; }

private:
  
};

#endif
