#include "Label.h"


BEGIN_GUI

Label::Label(const std::string& text,
	     StbFont stbfont,
	     const math::vec4& fontColor)
    :fontSize(20),
     fontColor(fontColor),
     stbfont(stbfont)
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
     stbfont(stbfont)
{
    
}


std::vector<DrawCommand>& Label::getDrawCmds()
{
    if(this->fontSize < 1.0) std::cout << "Warnning::Label::getDrawCmds: fontSize is too small!" << std::endl;
    this->drawCmds.clear();

    // TODO: Anchor and more align

    // Align text to center
    float cw = stbfont.getTextWidth(text, this->fontSize);
    float x = rect.x + (rect.w - cw) / 2.0f;
    x = std::max(x, rect.x);
    float ch = stbfont.getFontHeight(this->fontSize);
    float y = rect.y + (rect.h - ch) / 2.0f + ch;
    // Make text command
    for(wchar_t c : this->text)
    {
	PackedCharactor& pc = stbfont.getPackedCharactor(c, this->fontSize, &x, &y);
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
	gui::DrawCommand& cmd = GLRenderer::makeCharQuad(pc, this->fontColor, &this->rect);
	this->drawCmds.emplace_back(cmd);
    }
    return this->drawCmds;	// @IDrawable
}


END_GUI

