#include "Progressbar.h"

#include <cassert>


BEGIN_GUI

Progressbar::Progressbar(float min,
			 float max,
			 math::vec4 bgColor,
			 math::vec4 fillColor)
    :min(min),
     max(max),
     bgColor(bgColor),
     fillColor(fillColor),
     thickness(0),
     borderColor(.3, .3, .3, 1),
     radius(0),
     bgTexture(NULL),
     fillTexture(NULL),
     onChanged(NULL),
     value((min + max) / 2)
{
    assert(max > min);
}


std::vector<DrawCommand *>& Progressbar::getDrawCmds()
{
    this->clearDrawCmds();

    if(this->radius > 0)
    {
	// Make background rectangle command
	gui::DrawCommand *bg = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->bgColor, this->bgTexture);
	this->drawCmds.emplace_back(bg);
	
	// Make fill rectangle command
	// Rect rect_fill(rect.x, rect.y, (value - min) / (max - min) * rect.w, rect.h);
	Rect rect_fill(rect);
	rect_fill.scale((value - min) / (max - min), 1);
	gui::DrawCommand *fill = GLRenderer::makeFillRoundRect(rect_fill, this->radius, this->fillColor, this->fillTexture);
	this->drawCmds.emplace_back(fill);
	if(this->fillTexture)
	{
	    gui::DrawCommand *fill_bg = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->fillColor, this->fillTexture);
	    fill_bg->clip = new Rect(rect_fill);
	    this->drawCmds.emplace_back(fill_bg);
	}

	// Make stroke command
	if(this->thickness > 0)
	{
	    gui::DrawCommand *stroke = GLRenderer::makeRoundRect(this->rect, this->radius, this->borderColor, this->thickness);
	    this->drawCmds.emplace_back(stroke);
	}
    }
    else
    {
	// Make background rectangle command
	gui::DrawCommand *bg = GLRenderer::makeFillRect(this->rect, this->bgColor, this->bgTexture);
	this->drawCmds.emplace_back(bg);
	
	// Make fill rectangle command
	// Rect rect_fill(rect.x, rect.y, (value - min) / (max - min) * rect.w, rect.h);
	Rect rect_fill(rect);
	rect_fill.scale((value - min) / (max - min), 1);
	gui::DrawCommand *fill = GLRenderer::makeFillRect(rect_fill, this->fillColor, NULL);
	this->drawCmds.emplace_back(fill);
	if(this->fillTexture)
	{
	    gui::DrawCommand *fill_bg = GLRenderer::makeFillRect(this->rect, this->fillColor, this->fillTexture);
	    fill_bg->clip = new Rect(rect_fill);
	    this->drawCmds.emplace_back(fill_bg);
	}

	// Make stroke command
	if(this->thickness > 0)
	{
	    gui::DrawCommand *stroke = GLRenderer::makeRectangle(this->rect, this->borderColor, this->thickness);
	    this->drawCmds.emplace_back(stroke);
	}
    }

    return this->drawCmds;
}

float Progressbar::getValue()
{
    return this->value;
}

#define PB_CLAMP(v, min, max) (v < min ? min : (v > max ? max : v))

void Progressbar::operator=(float value)
{
    this->value = PB_CLAMP(value, min, max);
    if(this->onChanged) this->onChanged(this, this->value);
}

void Progressbar::operator+=(float value)
{
    float v = PB_CLAMP(this->value + value, min, max);
    this->value = v;
    if(this->onChanged) this->onChanged(this, this->value);
}

void Progressbar::operator-=(float value)
{
    float v = PB_CLAMP(this->value - value, min, max);
    this->value = v;
    if(this->onChanged) this->onChanged(this, this->value);
}

bool Progressbar::operator>(float value)
{
    return this->value > value;
}

bool Progressbar::operator<(float value)
{
    return this->value < value;
}

bool Progressbar::operator>=(float value)
{
    return this->value >= value;	
}

bool Progressbar::operator<=(float value)
{
    return this->value <= value;	
}

bool Progressbar::operator==(float value)
{
    return this->value == value;
}

    
END_GUI
