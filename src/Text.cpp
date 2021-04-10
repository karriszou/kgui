#include "Text.h"


BEGIN_GUI
    
Text::Text(const std::string& text,
	   StbFont font,
	   const math::vec4& fgColor,
	   const math::vec4& bgColor)
    :fontSize(20),
     fgColor(fgColor),
     bgColor(bgColor),
     radius(0),
     thickness(0),
     borderColor(),
     font(font),
     texture(NULL)
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

Text::Text(const std::wstring& text,
	   StbFont font,
	   const math::vec4& fgColor,
	   const math::vec4& bgColor)
    :text(text),
     fontSize(20),
     fgColor(fgColor),
     bgColor(bgColor),
     radius(0),
     thickness(0),
     borderColor(),
     font(font),
     texture(NULL)
{
    
}

void Text::setTexture(GLTexture *texture)
{
    this->texture = texture;
}

void Text::setTexture(int w, int h, unsigned char *data, GLTextureFormat format)
{
    this->texture = new GLTexture(w, h, data, format);
}

std::vector<DrawCommand>& Text::getDrawCmds()
{
    if(this->fontSize < 1.0) std::cout << "Warnning::Text::getDrawCmds: font size is too small!" << std::endl;
    this->drawCmds.clear();

    // TODO: Anchor and more align

    // Make filled-rectangle command
    if(this->radius > 0)
    {
	gui::DrawCommand& cmd = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->bgColor, this->texture);
	this->drawCmds.emplace_back(cmd);
    }
    else
    {
	// gui::VertexData vtxdata[] =
	// 	{
	// 	 { rect.x,		rect.y ,		0, 1,	bgColor.x, bgColor.y, bgColor.z, bgColor.w },
	// 	 { rect.x + rect.w,	rect.y ,		1, 1,	bgColor.x, bgColor.y, bgColor.z, bgColor.w },
	// 	 { rect.x + rect.w,	rect.y + rect.h,	1, 0,	bgColor.x, bgColor.y, bgColor.z, bgColor.w },
	// 	 { rect.x,		rect.y + rect.h,	0, 0,	bgColor.x, bgColor.y, bgColor.z, bgColor.w },
	// 	};
	// unsigned int indices[] =
	// 	{
	// 	 0, 1, 2,
	// 	 2, 3, 0
	// 	};
	// gui::DrawCommand cmd(DrawType::FillRect);
	// cmd.setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
	// cmd.setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
	// if(this->texture) cmd.setTexture(this->texture);
	// this->drawCmds.emplace_back(cmd);

	gui::DrawCommand& cmd = GLRenderer::makeFillRect(this->rect, this->bgColor, this->texture);
	this->drawCmds.emplace_back(cmd);
    }
	
    // Make rectangle(border) command
    if(this->thickness > 0)
    {
	// gui::VertexData vtxdata[] =
	// 	{
	// 	 { rect.x,		rect.y ,		0, 1,	borderColor.x, borderColor.y, borderColor.z, borderColor.w },
	// 	 { rect.x + rect.w,	rect.y ,		1, 1,	borderColor.x, borderColor.y, borderColor.z, borderColor.w },
	// 	 { rect.x + rect.w,	rect.y + rect.h,	1, 0,	borderColor.x, borderColor.y, borderColor.z, borderColor.w },
	// 	 { rect.x,		rect.y + rect.h,	0, 0,	borderColor.x, borderColor.y, borderColor.z, borderColor.w },
	// 	};
	// unsigned int indices[] = { 0, 1, 2, 3 };	
	// gui::DrawCommand cmd(DrawType::Rectangle);
	// cmd.setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
	// cmd.setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
	// cmd.lineSize = this->thickness;

	gui::DrawCommand& cmd = GLRenderer::makeRectangle(this->rect, this->borderColor, this->thickness);
	this->drawCmds.emplace_back(cmd);
    }

    // Make text command
    {
	// Align text to center
	float cw = this->font.getTextWidth(text, this->fontSize);
	float x = rect.x + (rect.w - cw) / 2.0f;
	x = std::max(x, rect.x);
	float ch = this->font.getFontHeight(this->fontSize);
	float y = rect.y + (rect.h - ch) / 2.0f + ch;
	for(wchar_t c : this->text)
	{
	    PackedCharactor& pc = font.getPackedCharactor(c, this->fontSize, &x, &y);
	    // gui::VertexData vtxdata[] =
	    //     {
	    //      { pc.x0, pc.y0,	pc.s0, pc.t0,	fgColor.x, fgColor.y, fgColor.z, fgColor.w },
	    //      { pc.x1, pc.y0,	pc.s1, pc.t0,	fgColor.x, fgColor.y, fgColor.z, fgColor.w },
	    //      { pc.x1, pc.y1,	pc.s1, pc.t1,	fgColor.x, fgColor.y, fgColor.z, fgColor.w },
	    //      { pc.x0, pc.y1,	pc.s0, pc.t1,	fgColor.x, fgColor.y, fgColor.z, fgColor.w },
	    //     };
	    // unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
	    // // gui::DrawCommand cmd(DrawType::Text);
	    // // Add :: operator to prevent compiler lookup Text(enum) in namaspace 'gui'
	    // gui::DrawCommand cmd(::Text);
	    // cmd.setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
	    // cmd.setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
	    // cmd.setTexture(pc.bw, pc.bh, pc.bimap, Alpha);
	    // cmd.clip = &rect;

	    gui::DrawCommand& cmd = GLRenderer::makeCharQuad(pc, this->fgColor, &this->rect);
	    this->drawCmds.emplace_back(cmd);
	}
    }
    return this->drawCmds;
}

// Rect *Text::getEvenRegion()
// {
// 	return &this->rect;	    
// }

// void Text::onEven(GUIEven& e)
// {
// 	(void)e;
// }

END_GUI
