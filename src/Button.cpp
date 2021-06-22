#include "Button.h"



namespace gui
{
    
    Button::Button(const std::string& text,
		   StbFont font,
		   const math::vec4& fgColor,
		   const math::vec4& bgColor)
	:fontSize(20),
	 fgColor(fgColor),
	 bgColor(bgColor),
	 radius(0),
	 thickness(0),
	 borderColor(),
	 onClick(nullptr),
	 onPress(nullptr),
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

    Button::Button(const std::wstring& text,
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
	 onClick(nullptr),
	 onPress(nullptr),
	 font(font),
	 texture(NULL)
    {
    
    }

    void Button::setTexture(GLTexture *texture)
    {
	this->texture = texture;
    }

    void Button::setTexture(int w, int h, unsigned char *data, GLTextureFormat format)
    {
	this->texture = new GLTexture(w, h, data, format);
    }

    std::vector<DrawCommand *>& Button::getDrawCmds()
    {
	if(this->fontSize < 1.0) std::cout << "Warnning::Text::getDrawCmds: font size is too small!" << std::endl;
	this->clearDrawCmds();

	// TODO: Anchor and more align

	// Make filled-rectangle command
	if(this->radius > 0)
	{
	    gui::DrawCommand *cmd = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->bgColor, this->texture);
	    this->drawCmds.emplace_back(cmd);
	}
	else
	{
	    gui::DrawCommand* cmd = GLRenderer::makeFillRect(this->rect, this->bgColor, this->texture);
	    this->drawCmds.emplace_back(cmd);
	}
	
	// Make rectangle(border) command
	if(this->thickness > 0)
	{
	    gui::DrawCommand* cmd = GLRenderer::makeRectangle(this->rect, this->borderColor, this->thickness);
	    this->drawCmds.emplace_back(cmd);
	}

	// Make text command
	if(this->text.length() > 0)
	{
	    // Align text to center
	    float cw = this->font.getTextWidth(text, this->fontSize);
	    float x = rect.x + (rect.w - cw) / 2.0f;
	    x = std::max(x, rect.x);
	    float ch = this->font.getFontHeight(this->fontSize);
	    float y = rect.y + (rect.h - ch) / 2.0f + ch;
	    for(wchar_t c : this->text)
	    {
		PackedCharactor *pc = font.getPackedCharactor(c, this->fontSize, &x, &y);
		gui::DrawCommand *cmd = GLRenderer::makeCharQuad(*pc, this->fgColor, &this->rect);
		this->drawCmds.emplace_back(cmd);
	    }
	}
	return this->drawCmds;
    }

    Rect *Button::getEvenRegion()
    {
	return &this->rect;
    }

    void Button::onEven(GUIEven& e)
    {
	// Click even
	if(e.isClick && this->onClick)
	{
	    this->onClick(this, e);
	}
	if(e.isPress && this->onPress)
	{
	    this->onPress(this, e);
	}

	// Hover even
	static math::vec4 bg = bgColor;
	if(e.isHover) bgColor = bg * 3;
	else bgColor = bg;

    }

}	// namespace gui
