#include "Slider.h"


namespace gui
{

    Slider::Slider(float min,
		   float max,
		   math::vec4 bgColor,
		   math::vec4 fillColor,
		   math::vec4 sliderColor)
	:min(min),
	 max(max),
	 bgColor(bgColor),
	 fillColor(fillColor),
	 sliderColor(sliderColor),
	 thickness(0),
	 borderColor(.3, .3, .3, 1),
	 radius(0),
	 bgTexture(NULL),
	 fillTexture(NULL),
	 onChanged(NULL),
	 onDrag(NULL),
	 value((min + max) / 2)
    {
	
    }



    std::vector<DrawCommand *>& Slider::getDrawCmds()
    {
	this->clearDrawCmds();

	Rect rect_fill(rect);
	rect_fill.scale((value - min) / (max - min), 1);
	// Rect rect_fill(rect.x, rect.y, (value - min) / (max - min) * rect.w, rect.h);

	float w, h; w = h = this->rect.h;
	float x = rect.x + ((value - min) / (max - min) * rect.w) - w / 2;
	Rect rect_slider(x, rect.y, w, h);

	if(this->radius > 0)
	{
	    // Make background rectangle command
	    gui::DrawCommand *bg = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->bgColor, this->bgTexture);
	    this->drawCmds.emplace_back(bg);
	
	    // Make fill rectangle command
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

	    // Make slider command
	    gui::DrawCommand *slider = GLRenderer::makeFillRoundRect(rect_slider, this->radius, this->sliderColor, NULL);
	    this->drawCmds.emplace_back(slider);
	}
	else
	{
	    // Make background rectangle command
	    gui::DrawCommand *bg = GLRenderer::makeFillRect(rect, this->bgColor, this->bgTexture);
	    this->drawCmds.emplace_back(bg);
	
	    // Make fill rectangle command
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

	    // Make slider rectangle command
	    gui::DrawCommand *slider = GLRenderer::makeFillRect(rect_slider, this->sliderColor, NULL);
	    this->drawCmds.emplace_back(slider);
	}

	return this->drawCmds;
    }

    float Slider::getValue()
    {
	return this->value;
    }

    Rect* Slider::getEvenRegion()
    {
	float w, h; w = h = this->rect.h;
	float x = rect.x + (value - min) / (max - min) * rect.w - w / 2;
	return new Rect(x, rect.y, w, h);
    }

#define SLIDER_CLAMP(v, min, max) (v < min ? min : (v > max ? max : v))

    void Slider::onEven(GUIEven& e)
    {
	static math::vec4 hover = this->sliderColor;
	if(e.isHover) this->sliderColor = hover * 1.2;
	else  this->sliderColor = hover;

	// On drag
	static bool drag = false;
	if(e.isPress) drag = true;
	if(e.left && rect.contain(e.cx, e.cy)) drag = true;
	if(drag)
	{
	    value = (e.cx - rect.x) / rect.w * (max - min) + min;
	    value = SLIDER_CLAMP(value, min, max);
	    // Notice
	    if(this->onChanged) this->onChanged(this, this->value);
	    if(this->onDrag) this->onDrag(this, e, this->value);
	    if(!e.left) drag = false;
	}

    }

    void Slider::operator=(float value)
    {
	this->value = SLIDER_CLAMP(value, min, max);
	if(this->onChanged) this->onChanged(this, this->value);
    }

    void Slider::operator+=(float value)
    {
	float v = SLIDER_CLAMP(this->value + value, min, max);
	this->value = v;
	if(this->onChanged) this->onChanged(this, this->value);
    }

    void Slider::operator-=(float value)
    {
	float v = SLIDER_CLAMP(this->value - value, min, max);
	this->value = v;
	if(this->onChanged) this->onChanged(this, this->value);
    }

    bool Slider::operator>(float value)
    {
	return this->value > value;
    }

    bool Slider::operator<(float value)
    {
	return this->value < value;
    }

    bool Slider::operator>=(float value)
    {
	return this->value >= value;	
    }

    bool Slider::operator<=(float value)
    {
	return this->value <= value;	
    }

    bool Slider::operator==(float value)
    {
	return this->value == value;
    }

    
}	// namespace gui
