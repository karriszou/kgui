#include "Label.h"
#include <memory>


BEGIN_GUI

Label::Label(const std::string& text,
	     StbFont stbfont,
	     const math::vec4& fontColor)
    :fontSize(20),
     fontColor(fontColor),
     stbfont(stbfont),
     hAlign(gui::Align::Center),
     vAlign(gui::Align::Center)
{
    if(text.size() > 0)
    {
	// make string reference become a lvalue in current stack
	const char *src = text.c_str();
	std::mbstate_t state = std::mbstate_t();
	size_t len = std::mbsrtowcs(NULL, (const char **)&text, 0, &state) /* + 1 */;
	this->text = *new std::wstring(len, L' ');
	std::mbsrtowcs(&this->text[0], &src, text.size(), &state);
    }
}

Label::Label(const std::wstring& text,
	     StbFont stbfont,
	     const math::vec4& fontColor)
    :text(text),
     fontSize(20),
     fontColor(fontColor),
     stbfont(stbfont),
     hAlign(gui::Align::Center),
     vAlign(gui::Align::Center)
{
    
}

void Label::setText(const std::string& text)
{
    // make string reference become a lvalue in current stack
    const char *src = text.c_str();
    std::mbstate_t state = std::mbstate_t();
    size_t len = std::mbsrtowcs(NULL, (const char **)&text, 0, &state) /* + 1 */;
    this->text.clear();
    this->text = *new std::wstring(len, L' ');
    std::mbsrtowcs(&this->text[0], &src, text.size(), &state);
}

void Label::align(gui::Align horzontal, gui::Align vertical)
{
    this->hAlign = horzontal;
    this->vAlign = vertical;
}

std::vector<DrawCommand *>& Label::getDrawCmds()
{
    if(this->fontSize < 1.0) std::cout << "Warnning::Label::getDrawCmds: fontSize is too small!" << std::endl;
    
    this->clearDrawCmds();

    float x, y;
    // Align text
    float width = this->stbfont.getTextWidth(text, this->fontSize);
    if(this->hAlign == gui::Align::Left)
    {
	x = rect.x;
    }
    else if(this->hAlign == gui::Align::Center)
    {
	x = rect.x + (rect.w - width) / 2.0f;
	x = std::max(x, rect.x);
    }
    else 
    {
	x = rect.x + (rect.w - width);
	x = std::max(x, rect.x);
	if(this->hAlign != gui::Align::Right)
	    std::cout << "Warnning::Text::getDrawCmds: Horizontal text align must be left, center or right!" << std::endl;
    }

    float ch = this->stbfont.getFontHeight(this->fontSize);
    if(this->vAlign == gui::Align::Top)
    {
	y = rect.y + ch;
    }
    else if(this->vAlign == gui::Align::Center)
    {
	y = rect.y + ch + (rect.h - ch) / 2.0f;
    }
    else 
    {
	y = rect.y + rect.h;
	if(this->vAlign != gui::Align::Bottom)
	    std::cout << "Warnning::Text::getDrawCmds: Vertical text align must be top, center or bottom!" << std::endl;
    }


    // Make text command
    for(wchar_t c : this->text)
    {
	std::shared_ptr<PackedCharactor> pc = stbfont.getPackedCharactor(c, this->fontSize, &x, &y);
	// gui::VertexData vtxdata[] =
	// 	{
	// 	 { pc.x0, pc.y0,	pc.s0, pc.t0,	fontColor.x, fontColor.y, fontColor.z, fontColor.w },
	// 	 { pc.x1, pc.y0,	pc.s1, pc.t0,	fontColor.x, fontColor.y, fontColor.z, fontColor.w },
	// 	 { pc.x1, pc.y1,	pc.s1, pc.t1,	fontColor.x, fontColor.y, fontColor.z, fontColor.w },
	// 	 { pc.x0, pc.y1,	pc.s0, pc.t1,	fontColor.x, fontColor.y, fontColor.z, fontColor.w },
	// 	};
	// unsigned int idxdata[] = { 0, 1, 2, 2, 3, 0 };
	// gui::DrawCommand cmd(Text);		// = *new gui::DrawCommand(Text);
	// cmd.setVtxData(vtxdata, 4);
	// cmd.setIdxData(idxdata, 6);
	// cmd.setTexture(pc.bw, pc.bh, pc.bimap, Alpha);
	// cmd.clip = &this->rect;

	gui::DrawCommand *cmd = GLRenderer::makeCharQuad(*pc, this->fontColor, &this->rect);
	this->drawCmds.emplace_back(cmd);
	// delete pc;
    }

    return this->drawCmds;	// @IDrawable
}


END_GUI

