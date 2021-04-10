#ifndef PROGRESSBAR_H
#define PROGRESSBAR_H


#include "GUI.h"
#include <functional>

BEGIN_GUI

class Progressbar : public GUI
{
public:
    float min, max;
    math::vec4 bgColor;
    math::vec4 fillColor;
    float thickness;
    math::vec4 borderColor;
    float radius;
    GLTexture *bgTexture;	
    GLTexture *fillTexture;
    std::function<void(void *sender, float value)> onChanged;

    Progressbar(float min,
		float max,
		math::vec4 bgColor = math::vec4(.1, .1, .1, .8),
		math::vec4 fillColor = math::vec4(.6, .6, .6, 1));


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

private:
    float value;
     
};
    
END_GUI

#endif
