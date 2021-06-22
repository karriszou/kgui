#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "GUI.h"
#include "StbFont.h"
#include <functional>


BEGIN_GUI

class Checkbox : public GUI
{
public:
    std::wstring text;
    math::vec4 bgColor;
    math::vec4 fillColor;
    math::vec4 fontColor;
    float fontSize;
    std::function<void(void *sender, bool value)> onCheck;

    Checkbox(std::wstring text,
	     StbFont stbfont,
	     math::vec4 bgColor = math::vec4(.1, .1, .1, 1),
	     math::vec4 fillColor = math::vec4(.8, .8, .8, 1));

    Checkbox(std::string text,
	     StbFont stbfont,
	     math::vec4 bgColor = math::vec4(.1, .1, .1, 1),
	     math::vec4 fillColor = math::vec4(.8, .8, .8, 1));

    void check();

    void uncheck();

    inline bool isChecked() { return this->value; }

    std::vector<DrawCommand *>& getDrawCmds() override;

    inline operator bool() { return this->value; }

protected:
    Rect* getEvenRegion() override;

    void onEven(GUIEven& e) override;

private:
    bool value;
    StbFont stbfont;
};
    

END_GUI

#endif
