#ifndef LABEL_H
#define LABEL_H

#include <string>
#include "Rect.h"
#include "GUI.h"
#include "StbFont.h"
#include "Vector.h"

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


    std::vector<DrawCommand>& getDrawCmds() override;

    // protected:
    // Rect *getEvenRegion() override;
    // void onEven(GUIEven& e) override;

private:
    StbFont stbfont;
};
    
END_GUI

#endif
