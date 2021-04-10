#ifndef GUI_H
#define GUI_H

#include <vector>
#include "Rect.h"
#include "Vector.h"
#include "GLRenderer.h"


#define BEGIN_GUI namespace gui {
#define END_GUI }


BEGIN_GUI


struct GUIEven;			// in self, 
struct GUIData;
struct VertexData;
struct DrawCommand;
    
class INotifiable
{
public:
    virtual Rect* getEvenRegion();
    virtual void onEven(GUIEven& even);
};


class IDrawable
{
public:
    virtual std::vector<DrawCommand>& getDrawCmds() = 0;
protected:
    std::vector<DrawCommand> drawCmds;
};


struct GUIEven
{
    float cx, cy, sx, sy;
    bool left, right;
    bool isHover, isClick, isPress;
    GUIEven() : cx(0), cy(0), sx(0), sy(0), left(false), right(false), isHover(false), isClick(false), isPress(false) { }
};


enum AnchorType
    {
     None,
     TopLeft,
     TopRight,
     BottomRight,
     BottomLeft,
     Left,
     Top,
     Right,
     Bottom,
     Center
    };


class Container;
class GUILayout
{
public:
    // Compute control's region by its parent container (if parent is NULL, then rect is GUI viewport)
    Rect computeRect();
    Rect computeRect(int x, int y, int width, int height);

    Rect getRect();

    // Int : fixed number Float: percentage number
    void layout(int x, int y, int w, int h, AnchorType type = AnchorType::None, Container *parent = NULL);

    void layout(int x, int y, float w, float h, AnchorType type, Container *parent = NULL);

    void layout(float x, float y, float w, float h, AnchorType type, Container *parent = NULL);

    void anchor(AnchorType type);

    void parent(const Container& parent);

protected:
    Rect rect;
    AnchorType type;
    const Container *container;

    GUILayout();
private:
    bool isFixedX;
    bool isFixedY;
    bool isFixedW;
    bool isFixedH;
    float x, y, w, h;
};


//
// GUI
//
class GUI : public GUILayout, public IDrawable, public INotifiable
{
public:
    // static void add(int count, GUI *gui...);

    static void update(int width, int height, double cursorX, double cursorY, float scrollX, float scrollY, bool leftMouse, bool rightMouse);

    static void render();

protected:
    GUI();
    ~GUI();
    static int width, height;

private:
    static std::vector<GUI *> childrens;
    static GLRenderer *renderer;
};


//
// Container
//
class Container : public GUI
{
public:
    math::vec4 bgColor;
    math::vec4 borderColor;
    float thickness;
    float radius;
    GLTexture *texture;
    
    Container(math::vec4 bgColor = math::vec4(.1),
	      math::vec4 borderColor = math::vec4(.4, .4, .4, 1),
	      float thickness = 1,
	      float radius = 0);

    std::vector<DrawCommand>& getDrawCmds();

private:

};


struct GUIData
{
    int width;
    int height;
    double cursorX;
    double cursorY;
    float scrollX;
    float scrollY;
    bool leftMouse;
    bool middleMouse;
    bool rightMouse;
};


class IGroupable
{
public:
    std::string groupName;
protected:
    IGroupable();
    ~IGroupable();
    void sendGroupMessage(int msgCode);
    virtual void onGroupMessage(int msgCode) = 0;
private:
    static std::vector<IGroupable *> members;
};


struct VertexData
{
    float x, y;
    float u, v;
    float r, g, b, a;
};


struct DrawCommand
{
    DrawType type;
    std::vector<VertexData> vtxData;
    std::vector<unsigned int> idxData;
    GLTexture *texture;
    math::vec4 color;		// uniform
    float factor;			// uniform
    Rect *clip;
    float pointSize;		// gl attribute
    float lineSize;			// gl attribute

    explicit
    DrawCommand(DrawType type,
		GLTexture *texture = NULL,
		math::vec4 color = math::vec4(1.0),
		float factor = 1.0,
		float psize = 1.0,
		float lsize = 1.0)
	:type(type),
	 vtxData(),
	 idxData(),
	 texture(texture),
	 color(color),
	 factor(factor),
	 clip(NULL),
	 pointSize(psize),
	 lineSize(lsize)
    {
	
    }

    DrawCommand(DrawType type,
		const std::vector<VertexData>& vdata,
		const std::vector<unsigned int>& idata,
		GLTexture *texture = NULL,
		math::vec4 color = math::vec4(1.0),
		float factor = 1.0,
		float psize = 1.0,
		float lsize = 1.0)
	:type(type),
	 vtxData(vdata),
	 idxData(idata),
	 texture(texture),
	 color(color),
	 factor(factor),
	 clip(NULL),
	 pointSize(psize),
	 lineSize(lsize)
    {
	
    }

    void setVtxData(VertexData *vdata, int count);

    void setIdxData(unsigned int *idata, int count);

    void setTexture(GLTexture *texture);

    void setTexture(int w, int h, unsigned char *data, GLTextureFormat format = (GLTextureFormat)0);

};

    
END_GUI

#endif
