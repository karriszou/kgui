#ifndef BUTTON_H
#define BUTTON_H

#include "GUI.h"
#include "StbFont.h"

#include <functional>


BEGIN_GUI

class Button : public GUI
{
public:
    std::wstring text;
    float fontSize;
    math::vec4 fgColor;
    math::vec4 bgColor;
    float radius;
    float thickness;
    math::vec4 borderColor;
    std::function<void(void *sender, GUIEven e)> onClick;
    std::function<void(void *sender, GUIEven e)> onPress;

    Button(const std::string& text,
	   StbFont font,
	   const math::vec4& fgColor = math::vec4(0, 0, 0, 1),
	   const math::vec4& bgColor = math::vec4(1));
	
    Button(const std::wstring& text,
	   StbFont font,
	   const math::vec4& fgColor = math::vec4(0, 0, 0, 1),
	   const math::vec4& bgColor = math::vec4(1));

    void setTexture(GLTexture *texture);

    void setTexture(int w, int h, unsigned char *data, GLTextureFormat format = GLTextureFormat::Rgba);

    std::vector<DrawCommand *>& getDrawCmds() override;


protected:
    Rect *getEvenRegion() override;
    void onEven(GUIEven& e) override;

private:
    StbFont font;
    GLTexture *texture;
};


END_GUI

#endif
