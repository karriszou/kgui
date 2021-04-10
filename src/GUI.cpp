#include "GUI.h"
#include <cassert>
#include <cstdarg>
#include <algorithm>


BEGIN_GUI

//
// GUI
//
std::vector<GUI*> GUI::childrens;
GLRenderer *GUI::renderer = NULL;
int GUI::width  = 0;
int GUI::height = 0;

GUI::GUI()
{
    GUI::childrens.emplace_back(this);
}

void GUI::update(int width, int height, double cursorX, double cursorY, float scrollX, float scrollY, bool leftMouse, bool rightMouse)
{
    GUI::width = width; GUI::height = height;

    // Layout
    for(GUI *gui : GUI::childrens)
    {
	assert(gui != NULL);
	if(gui->container != NULL)
	{
	    gui->computeRect();
	}
	else
	{
	    gui->computeRect(0, 0, width, height);
	}
    }

    // Set even variables by gui's even region
    static bool preLeft = false;
    for(GUI *gui : GUI::childrens)
    {
	assert(gui); GUIEven e;
	e.cx = cursorX; e.cy = cursorY;
	e.sx = scrollX; e.sy = scrollY;
	e.left = leftMouse; e.right = rightMouse;

	Rect *rect = gui->getEvenRegion();

	if(rect)
	{
	    e.isHover = rect->contain(cursorX, cursorY);
	    e.isClick = e.isHover && leftMouse;
	    e.isPress = preLeft && e.isClick;
	    e.isClick &= !e.isPress;
	}
	gui->onEven(e);
    }
    preLeft = leftMouse;
}

// void GUI::add(int count, GUI *gui...)
// {
// 	va_list ap;
// 	GUI::childrens.emplace_back(gui);
// 	va_start(ap, gui);
// 	for(int i = 1; i < count; i++)
// 	{
// 	    GUI *ui = va_arg(ap, GUI*);
// 	    assert(ui);
// 	    GUI::childrens.emplace_back(ui);
// 	    assert(GUI::childrens.size() < 1024);
// 	}
// 	va_end(ap);
// }

void GUI::render()
{
    // Renderer
    if(!GUI::renderer)
	GUI::renderer = new GLRenderer(0, 0, width, height);

    // Set renderer viewport
    GUI::renderer->viewPort(0, 0, width, height);

    for(GUI *gui : GUI::childrens)
    {
	assert(gui);
	GUI::renderer->draw(*gui);
    }
}

Rect* INotifiable::getEvenRegion()
{
    return NULL;
}
    
void INotifiable::onEven(GUIEven& even)
{
    (void)even;
}

GUI::~GUI()
{
    std::vector<GUI *>::iterator it = std::find(GUI::childrens.begin(), GUI::childrens.end(), this);
    if(it != GUI::childrens.end())
    {
	GUI::childrens.erase(it);
    }
    else
    {
	std::cout << "Error::GUI::Dtor: A children has not been removed!" << std::endl;
    }
}


//
// Container
//
Container::Container(math::vec4 bgColor,
		     math::vec4 borderColor,
		     float thickness,
		     float radius)
    :bgColor(bgColor),
     borderColor(borderColor),
     thickness(thickness),
     radius(radius),
     texture(NULL)
{

}

std::vector<DrawCommand>& Container::getDrawCmds()
{
    this->drawCmds.clear();

    if(this->radius > 0)
    {
	// Make background fill rect command
	gui::DrawCommand& bg = GLRenderer::makeFillRoundRect(this->rect, this->radius, this->bgColor, this->texture);
	this->drawCmds.emplace_back(bg);

	// Make border command
	gui::DrawCommand& border = GLRenderer::makeRoundRect(this->rect, this->radius, this->borderColor, this->thickness);
	this->drawCmds.emplace_back(border);
    }
    else
    {
	// Make background fill rect command
	gui::DrawCommand& bg = GLRenderer::makeFillRect(this->rect, this->bgColor, this->texture);
	this->drawCmds.emplace_back(bg);

	// Make border command
	gui::DrawCommand& border = GLRenderer::makeRectangle(this->rect, this->borderColor, this->thickness);
	this->drawCmds.emplace_back(border);
    }

    return this->drawCmds;
}


//
// GUILayout
//
GUILayout::GUILayout()
    :rect(),
     type(AnchorType::None),
     container(NULL),
     isFixedX(true),
     isFixedY(true),
     isFixedW(true),
     isFixedH(true),
     x(0), y(0), w(80), h(30)
{
	
}

Rect GUILayout::computeRect()
{
    assert(this->container != NULL);
    const Rect *parent = &this->container->rect;
    return this->computeRect(parent->x, parent->y, parent->w, parent->h);
}

Rect GUILayout::computeRect(int x, int y, int width, int height)
{
    // if(this->parent)
    // {
    // 	x	= this->parent->rect.x;
    // 	y	= this->parent->rect.y;
    // 	width	= this->parent->rect.w;
    // 	height	= this->parent->rect.h;
    // }

    float rx = isFixedX ? this->x : this->x * width;
    float ry = isFixedY ? this->y : this->y * height;
    float rw = isFixedW ? this->w : this->w * width;
    float rh = isFixedH ? this->h : this->h * height;

    switch(type)
    {
    case TopLeft:
	break;

    case TopRight:
	rx = width - rx - rw;
	break;

    case BottomRight:
	rx = width  - rx - rw;
	ry = height - ry - rh;
	break;

    case BottomLeft:
	ry = height - ry - rh;
	break;

    case Left:
	ry = height / 2 - ry - rh / 2;
	break;

    case Top:
	rx = width / 2 + rx - rw / 2;
	break;

    case Right:
	rx = width - rx - rw;
	ry = height / 2 - ry - rh / 2;
	break;

    case Bottom:
	rx = width / 2 + rx - rw / 2;
	ry = height - ry - rh;
	break;

    case Center:
	rx = width  / 2 + rx - rw / 2;
	ry = height / 2 - ry - rh / 2;
	break;

    case None:
	break;
    }

    this->rect.set(x + rx, y + ry, rw, rh);
    return this->rect;
}

Rect GUILayout::getRect()
{
    return this->rect;
}

void GUILayout::anchor(AnchorType type)
{
    this->type = type;
}

void GUILayout::parent(const Container& parent)
{
    this->container = &parent;
}

#define LAYOUT_CLAMP(v, min, max) (v < min ? min : (v > max ? max : v))

void GUILayout::layout(int x, int y, int w, int h, AnchorType type, Container *parent)
{
    isFixedX = isFixedY = isFixedW = isFixedH = true;
    this->x = x; this->y = y; this->w = w; this->h = h;
    if(type != AnchorType::None) this->type = type;
    if(parent != NULL) this->container = parent;
}

void GUILayout::layout(int x, int y, float w, float h, AnchorType type, Container *parent)
{
    w = LAYOUT_CLAMP(w, -1, 1);
    h = LAYOUT_CLAMP(h, -1, 1);
    isFixedX = isFixedY = true;
    isFixedW = isFixedH = false;
    this->x = x; this->y = y; this->w = w; this->h = h;
    if(type != AnchorType::None) this->type = type;
    if(parent != NULL) this->container = parent;
}

void GUILayout::layout(float x, float y, float w, float h, AnchorType type, Container *parent)
{
    x = LAYOUT_CLAMP(x, -1, 1);
    y = LAYOUT_CLAMP(y, -1, 1);
    w = LAYOUT_CLAMP(w, -1, 1);
    h = LAYOUT_CLAMP(h, -1, 1);
    isFixedX = isFixedY = isFixedW = isFixedH = false;
    this->x = x; this->y = y; this->w = w; this->h = h;
    if(type != AnchorType::None) this->type = type;
    if(parent != NULL) this->container = parent;
}


//
// IGroupable
//
std::vector<IGroupable *> IGroupable::members;

IGroupable::IGroupable()
{
    IGroupable::members.emplace_back(this);
}

void IGroupable::sendGroupMessage(int msgCode)
{
    for(IGroupable *m : IGroupable::members)
    {
	assert(m);
	// Look up the same group
	if(this->groupName == m->groupName)
	{
	    // Do not send to self
	    if(this != m)
	    {
		m->onGroupMessage(msgCode);
	    }
	}
    }
}

IGroupable::~IGroupable()
{
    std::vector<IGroupable *>::iterator it =  std::find(IGroupable::members.begin(), IGroupable::members.end(), this);
    if(it != IGroupable::members.end())
    {
	IGroupable::members.erase(it);
    }
    else
    {
	std::cout << "Error::IGroupable::Dtor: A member is could not remove!: " << *it << std::endl;
    }
}


//
// DrawCommand
//
void DrawCommand::setVtxData(VertexData *vdata, int count)
{
    vtxData.assign(vdata, vdata + count);
}

void DrawCommand::setIdxData(unsigned int *idata, int count)
{
    idxData.assign(idata, idata + count);
}

void DrawCommand::setTexture(GLTexture *texture)
{
    this->texture = texture;
}

void DrawCommand::setTexture(int w, int h, unsigned char *data, GLTextureFormat format)
{
    this->texture = new GLTexture(w, h, data, format);
}

    
END_GUI
