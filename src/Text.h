#ifndef TEXT_H
#define TEXT_H

#include <string>
#include "Rect.h"
#include "GUI.h"
#include "StbFont.h"


struct GLTexture;

BEGIN_GUI
    
class Text : public GUI
{
public:
    // Rect rect;
    std::wstring text;
    float fontSize;
    math::vec4 fgColor;
    math::vec4 bgColor;
    float radius;
    float thickness;
    math::vec4 borderColor;

    Text(const std::string& text,
	 StbFont font,
	 const math::vec4& fgColor = math::vec4(.3, .3, .3, 1),
	 const math::vec4& bgColor = math::vec4(1));
    
    Text(const std::wstring& text,
	 StbFont font,
	 const math::vec4& fgColor = math::vec4(.3, .3, .3, 1),
	 const math::vec4& bgColor = math::vec4(1));

    void setText(const std::string& text);

    void align(gui::Align horzontal = gui::Align::Center, gui::Align vertical = gui::Align::Center);

    void setTexture(GLTexture *texture);

    void setTexture(int w, int h, unsigned char *data, GLTextureFormat format = GLTextureFormat::Rgba);

    std::vector<DrawCommand>& getDrawCmds() override;

    // protected:
    // 	Rect *getEvenRegion() override;
    // 	void onEven(GUIEven& e) override;

private:
    StbFont font;
    GLTexture *texture;
    gui::Align hAlign, vAlign;
};
    
END_GUI

#endif
