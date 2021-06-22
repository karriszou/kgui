#include "Radio.h"


BEGIN_GUI

Radio::Radio(std::wstring text,
	     StbFont stbfont,
	     std::string groupName,
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
    this->groupName = groupName;	// non-static member of base class initialize in derive class ctor
}

Radio::Radio(std::string text,
	     StbFont stbfont,
	     std::string groupName,
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
    this->groupName = groupName;
}

void Radio::check()
{
    this->value = true;
    if(this->onCheck)
	this->onCheck(this, this->value);
    // IGroupable send message
    this->sendGroupMessage(1);
}

void Radio::uncheck()
{
    this->value = false;
    if(this->onCheck)
	this->onCheck(this, this->value);
}

std::vector<DrawCommand *>& Radio::getDrawCmds()
{
    this->clearDrawCmds();

    // Make check border command
    Rect rect_border(rect.x, rect.y, rect.h, rect.h);
    gui::DrawCommand *border = GLRenderer::makeFillCircle(rect_border.center().x, rect_border.center().y, rect_border.w / 2, this->bgColor);
    this->drawCmds.emplace_back(border);

    // Make check circle command
    if(this->value)
    {
	Rect rect_check(rect_border);
	rect_check.scaleFromCenter(0.6, 0.6);
	gui::DrawCommand *check = GLRenderer::makeFillCircle(rect_check.center().x, rect_check.center().y, rect_check.w / 2, this->fillColor);
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
	    PackedCharactor *pc = stbfont.getPackedCharactor(c, this->fontSize, &x, &y);
	    gui::DrawCommand *cmd = GLRenderer::makeCharQuad(*pc, this->fontColor, rect_text);
	    this->drawCmds.emplace_back(cmd);
	    delete pc;
	}
    }

    return this->drawCmds;
}

Rect* Radio::getEvenRegion()
{
    return  new Rect(rect.x, rect.y, rect.h, rect.h);
}

void Radio::onEven(GUIEven& e)
{
    if(e.isClick)
    {
	if(!this->value)
	    this->check();
	// else this->uncheck();
    }
}

void Radio::onGroupMessage(int msgCode)
{
    if(msgCode)
    {
	this->uncheck();
    }
}

    
END_GUI
