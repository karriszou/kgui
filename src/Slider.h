#ifndef SLIDER_H
#define SLIDER_H

#include "GUI.h"
#include <functional>


BEGIN_GUI

class Slider : public GUI
{
public:
    float min, max;
    math::vec4 bgColor;
    math::vec4 fillColor;
    math::vec4 sliderColor;
    float thickness;
    math::vec4 borderColor;
    float radius;
    GLTexture *bgTexture;	
    GLTexture *fillTexture;
    std::function<void(void *sender, float value)> onChanged;
    std::function<void(void *sender, GUIEven e, float value)> onDrag;

    Slider(float min,
	   float max,
	   math::vec4 bgColor = math::vec4(.1, .1, .1, .8),
	   math::vec4 fillColor = math::vec4(.6, .6, .6, 1),
	   math::vec4 sliderColor = math::vec4(.8, .8, .8, 1));


    std::vector<DrawCommand>& getDrawCmds() override;

    float getValue();

    void operator=(float value);
    void operator+=(float value);
    void operator-=(float value);
    bool operator>(float value);
    bool operator<(float value);
    bool operator>=(float value);
    bool operator<=(float value);
    bool operator==(float value);

protected:
    Rect* getEvenRegion() override;
    void onEven(GUIEven& even) override;

private:
    float value;
};

    
END_GUI

#endif
