#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "Rect.h"
#include "GUI.h"
#include "StbFont.h"


BEGIN_GUI

struct DrawCommand;

class Label : public GUI
{
public:
    std::wstring text;
    float fontSize;
    math::vec4 fontColor;


    Label(const std::string& text,
	  StbFont stbfont,
	  const math::vec4& fontColor = math::vec4(1.0));
    
    Label(const std::wstring& text,
	  StbFont stbfont,
	  const math::vec4& fontColor = math::vec4(1.0));

    void setText(const std::string& text);

    void align(gui::Align horzontal = gui::Align::Center, gui::Align vertical = gui::Align::Center);

    std::vector<DrawCommand *>& getDrawCmds() override;

    // protected:
    // Rect *getEvenRegion() override;
    // void onEven(GUIEven& e) override;

private:
    StbFont stbfont;
    gui::Align hAlign, vAlign;
};
    
END_GUI

#endif
