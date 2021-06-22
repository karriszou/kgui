#include "Checkbox.h"


BEGIN_GUI

Checkbox::Checkbox(std::wstring text,
		   StbFont stbfont,
		   math::vec4 bgColor,
		   math::vec4 fillColor)
    :text(text),
     bgColor(bgColor),
     fillColor(fillColor),
     fontColor(1),
     fontSize(20),
     onCheck(NULL),
     value(false),
     stbfont(stbfont)
{
	
}

Checkbox::Checkbox(std::string text,
		   StbFont stbfont,
		   math::vec4 bgColor,
		   math::vec4 fillColor)
    :bgColor(bgColor),
     fillColor(fillColor),
     fontColor(1),
     fontSize(20),
     onCheck(NULL),
     value(false),
     stbfont(stbfont)
{
    if(text.size() > 0)
    {
	const char *src = text.c_str();
	std::mbstate_t state = std::mbstate_t();
	size_t len = std::mbsrtowcs(NULL, (const char **)&text, 0, &state) /* + 1 */;
	this->text = *new std::wstring(len, L' ');
	std::mbsrtowcs(&this->text[0], &src, text.size(), &state);
    }
}

void Checkbox::check()
{
    this->value = true;
    if(this->onCheck)
	this->onCheck(this, this->value);
}

void Checkbox::uncheck()
{
    this->value = false;
    if(this->onCheck)
	this->onCheck(this, this->value);
}

std::vector<DrawCommand *>& Checkbox::getDrawCmds()
{
    this->clearDrawCmds();

    // Make check border command
    Rect rect_border(rect.x, rect.y, rect.h, rect.h);
    gui::DrawCommand *border = GLRenderer::makeFillRoundRect(rect_border, rect_border.w / 9, this->bgColor);
    this->drawCmds.emplace_back(border);

    // Make check square command
    if(this->value)
    {
	Rect rect_check(rect_border);
	rect_check.scaleFromCenter(0.6, 0.6);
	gui::DrawCommand *check = GLRenderer::makeFillRoundRect(rect_check, rect_check.w / 9, this->fillColor);
	this->drawCmds.emplace_back(check);
    }

    // Make text command
    if(this->text.length() > 0)
    {
	// Align to center
	float text_width = stbfont.getTextWidth(text, this->fontSize);
	float x = rect_border.right() + (rect.w - rect_border.w - text_width) / 2;
	x = std::max(x, rect.x + rect_border.w);
	float font_height = stbfont.getFontHeight(this->fontSize);
	float y = rect.y + (rect.h - font_height) / 2.0f + font_height;
	Rect *rect_text = new Rect(x, y - font_height, this->rect.w - rect_border.w, this->rect.h);
	for(wchar_t c : this->text)
	{
	    std::shared_ptr<PackedCharactor> pc = stbfont.getPackedCharactor(c, this->fontSize, &x, &y);
	    gui::DrawCommand *cmd = GLRenderer::makeCharQuad(*pc, this->fontColor, rect_text);
	    this->drawCmds.emplace_back(cmd);
	}
    }

    return this->drawCmds;
}

Rect* Checkbox::getEvenRegion()
{
    return  new Rect(rect.x, rect.y, rect.h, rect.h);
}

void Checkbox::onEven(GUIEven& e)
{
    if(e.isClick)
    {
	if(this->value) this->uncheck();
	else this->check();
    }
}

    
END_GUI
