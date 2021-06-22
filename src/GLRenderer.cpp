#include "GLRenderer.h"
#include "GUI.h"

GLShader *GLRenderer::UIShader;
GLShader *GLRenderer::TextShader;
GLShader *GLRenderer::TextureShader;


GLRenderer::    GLRenderer(float x, float y, float w, float h)
    :x(x), y(y), w(w), h(h),
     vao(-1),
     vbo(-1)
{
    if(!gladLoadGL())
    {
	std::cerr << "Error::GLRenderer::Could not load OpenGL!" << std::endl;
	return;
    }

    if(GLRenderer::UIShader == NULL)
    	GLRenderer::UIShader = new GLShader(GLRenderer::ui_vs, GLRenderer::ui_fs);
    if(GLRenderer::TextShader == NULL)
    	GLRenderer::TextShader = new GLShader(GLRenderer::ui_vs, GLRenderer::ui_text_fs);
    if(GLRenderer::TextureShader == NULL)
    	GLRenderer::TextureShader = new GLShader(GLRenderer::ui_vs, GLRenderer::ui_texture_fs);

    glGenVertexArrays(1, &this->vao);
    glBindVertexArray(this->vao);
    glGenBuffers(1, &this->vbo);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    // glBufferData(GLenum target, GLsizeiptr size, const void *data, GLenum usage);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(gui::VertexData), (void*)offsetof(gui::VertexData, x));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(gui::VertexData), (void*)offsetof(gui::VertexData, u));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(gui::VertexData), (void*)offsetof(gui::VertexData, r));
}

struct glStatus
{
    GLboolean blend, cullFace, depthTest;
    GLint blendFunc, blendSrc, blendDst, polygonModes[2];
};

glStatus getGLStatus()
{
    glStatus status;
    glGetBooleanv(GL_BLEND, &status.blend);
    glGetBooleanv(GL_CULL_FACE, &status.cullFace);
    glGetBooleanv(GL_DEPTH_TEST, &status.depthTest);
    glGetIntegerv(GL_BLEND_EQUATION, &status.blendFunc);
    glGetIntegerv(GL_BLEND_SRC_ALPHA, &status.blendSrc);
    glGetIntegerv(GL_BLEND_DST_ALPHA, &status.blendDst);
    glGetIntegerv(GL_POLYGON_MODE, status.polygonModes);
    return status;
}

void setGLStatus(glStatus status)
{
    if (status.blend)		glBlendFunc(status.blendSrc, status.blendDst);
    else			glDisable(GL_BLEND);
    if (status.cullFace)	glEnable(GL_CULL_FACE);
    if (status.depthTest)	glEnable(GL_DEPTH_TEST);
    glBlendEquation(status.blendFunc);
    glPolygonMode(GL_FRONT_AND_BACK, status.polygonModes[0]);
}

void GLRenderer::draw(const gui::DrawCommand* cmd)
{
    // Fill buffer data
    glBindVertexArray(this->vao);
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(gui::VertexData) * cmd->vtxData.size(), &cmd->vtxData[0], GL_STATIC_DRAW);

    // Extract gl status
    glStatus status = getGLStatus();

    // Set gl status
    glEnable(GL_BLEND);
    glBlendEquation(GL_FUNC_ADD);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    bool enableTex = cmd->texture != NULL;

    // Set shader state
    GLShader *shader = NULL;
    if(cmd->type == Text)
	shader = GLRenderer::TextShader;
    else if(enableTex)
	shader = GLRenderer::TextureShader;
    else
	shader = GLRenderer::UIShader;

    if(shader == NULL)
    {
	std::cerr << "Error::GLRenderer::draw: shader could not be null!" << std::endl;
	return;
    }

    if(cmd->clip != NULL)
    {
	glEnable(GL_SCISSOR_TEST);
	// glScissor(cmd->clip->x, cmd->clip->y, cmd->clip->w, cmd->clip->h);
	glScissor(cmd->clip->x, this->h - cmd->clip->h - cmd->clip->y, cmd->clip->w, cmd->clip->h);	// Flip vertical
    }

    float ortho[4][4] =
	 {{ 2.0f,  0.0f,  0.0f, 0.0f},
	  { 0.0f, -2.0f,  0.0f, 0.0f},
	  { 0.0f,  0.0f, -1.0f, 0.0f},
	  {-1.0f,  1.0f,  0.0f, 1.0f}};
    ortho[0][0] /= this->w;
    ortho[1][1] /= this->h;

    shader->use();
    shader->setMat4("projection", ortho);
    shader->setVec4("color", cmd->color.get());
    shader->setFloat("factor", cmd->factor);
    
    if(enableTex)
    {
	// std::cout << cmd->texture->id << std::endl;
    	shader->setInt("image", 0);
    	glActiveTexture(GL_TEXTURE0);
    	glBindTexture(GL_TEXTURE_2D, cmd->texture->id);
    }

    // Draw call by difference drawtype
    switch(cmd->type)
    {
    case Point:
	glPointSize(cmd->pointSize);
	glDrawElements(GL_POINTS, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
	glPointSize(1.0);
    	break;

    case Line:
    	break;

    case Triangle:
    	break;

    case Rectangle:
	glLineWidth(cmd->lineSize);
	glDrawElements(GL_LINE_LOOP, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
	glLineWidth(1.0);
    	break;

    case RoundRect:
	glLineWidth(cmd->lineSize);
	glDrawElements(GL_LINE_LOOP, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
	glLineWidth(1.0);
    	break;

    case Circle:
	glLineWidth(cmd->lineSize);
	glDrawElements(GL_LINE_LOOP, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
	glLineWidth(1.0);
    	break;

    case Ellipse:
    	break;
    default:
    case FillTriangle:
    	break;
    case FillRect:
	glDrawElements(GL_TRIANGLES, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
    	break;

    case FillRoundRect:
	glDrawElements(GL_TRIANGLE_FAN, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
    	break;

    case FillCircle:
	glDrawElements(GL_TRIANGLE_FAN, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
    	break;

    case FillEllipse:
    	break;
    case Text:
	glDrawElements(GL_TRIANGLES, cmd->idxData.size(), GL_UNSIGNED_INT, &cmd->idxData[0]);
	break;
    }

    if(cmd->clip) glDisable(GL_SCISSOR_TEST);
    shader->use(false);

    // Restore gl status
    setGLStatus(status);
}

void GLRenderer::draw(gui::IDrawable& drawable)
{
    for(gui::DrawCommand* cmd : drawable.getDrawCmds())
    {
    	this->draw(cmd);
    }
}

gui::DrawCommand* GLRenderer::makeRectangle(const Rect& rect, math::vec4 color, float lineSize)
{
    gui::VertexData vtxdata[] =
	{{ rect.x,		rect.y ,		0, 1,	color.x, color.y, color.z, color.w },
	 { rect.x + rect.w,	rect.y ,		1, 1,	color.x, color.y, color.z, color.w },
	 { rect.x + rect.w,	rect.y + rect.h,	1, 0,	color.x, color.y, color.z, color.w },
	 { rect.x,		rect.y + rect.h,	0, 0,	color.x, color.y, color.z, color.w }};
    unsigned int indices[] = { 0, 1, 2, 3 };	
    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::Rectangle);
    cmd->setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
    cmd->setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
    cmd->lineSize = lineSize;
    return cmd;
}

gui::DrawCommand* GLRenderer::makeFillRect(const Rect& rect, math::vec4 color, GLTexture *texture)
{
    bool flip = true;
    gui::VertexData vtxdata[] =
	{{ rect.x,		rect.y ,		0, flip ? 0.f : 1.f,	color.x, color.y, color.z, color.w },
	 { rect.x + rect.w,	rect.y ,		1, flip ? 0.f : 1.f,	color.x, color.y, color.z, color.w },
	 { rect.x + rect.w,	rect.y + rect.h,	1, flip ? 1.f : 0.f,	color.x, color.y, color.z, color.w },
	 { rect.x,		rect.y + rect.h,	0, flip ? 1.f : 0.f,	color.x, color.y, color.z, color.w }};
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::FillRect);
    cmd->setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
    cmd->setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
    if(texture) cmd->setTexture(texture);
    return cmd;
}

gui::DrawCommand* GLRenderer::makeCircle(float x, float y, float r, math::vec4 color, float lineSize, int segment)
{
    std::vector<gui::VertexData> vtxdata;
    std::vector<unsigned int> indices;
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float d = 0, step = 2 * pi / segment;
    for(int i = 0; i < segment; i++)
    {
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	vtxdata.push_back({ vx, vy, 0, 0, color.x, color.y, color.z, color.w });
	indices.push_back(i);
	d += step;
    }
    gui::DrawCommand* cmd = new gui::DrawCommand(DrawType::Circle, vtxdata, indices);
    cmd->lineSize = lineSize;
    return cmd;
}

gui::DrawCommand* GLRenderer::makeFillCircle(float x, float y, float r, math::vec4 color, GLTexture *texture, int segment)
{
    std::vector<gui::VertexData> vtxdata;
    std::vector<unsigned int> indices;
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float d = 0, step = 2 * pi / segment;
    for(int i = 0; i < segment; i++)
    {
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	float u  = 0.5 * cosf(d) + 0.5;
	float v  = 0.5 * sinf(d) + 0.5;
	vtxdata.push_back({ vx, vy, u, v, color.x, color.y, color.z, color.w });
	indices.push_back(i + 1);
	d += step;
    }
    vtxdata.push_back({ x + r, y, 1, 0.5, color.x, color.y, color.z, color.w });
    indices.push_back(indices.size());
    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::FillCircle, vtxdata, indices);
    if(texture) cmd->setTexture(texture);
    return cmd;
}

gui::DrawCommand* GLRenderer::makeRoundRect(const Rect& rect, float r, math::vec4 color, float lineSize, int segment)
{
    r = r > std::min(rect.w / 2.0f, rect.h / 2.0f) ? std::min(rect.w / 2.0f, rect.h / 2.0f) : r;

    // gui::VertexData topLT = { rect.x + r,	rect.y,			0, 0,	color.x, color.y, color.z, color.w };
    // gui::VertexData botLT = { rect.x, rect.y + r,			0, 0,	color.x, color.y, color.z, color.w };

    // gui::VertexData topRT = { rect.x + rect.w - r, rect.y,		0, 0,	color.x, color.y, color.z, color.w };
    // gui::VertexData botRT = { rect.x + rect.w, rect.y + r,		0, 0,	color.x, color.y, color.z, color.w };

    // gui::VertexData topRB = { rect.x + rect.w, rect.y + rect.h - r,	0, 0,	color.x, color.y, color.z, color.w };
    // gui::VertexData botRB = { rect.x + rect.w - r, rect.y + rect.h,		0, 0,	color.x, color.y, color.z, color.w };

    // gui::VertexData topLB = { rect.x, rect.y + rect.h - r,		0, 0,	color.x, color.y, color.z, color.w };
    // gui::VertexData botLB = { rect.x + r, rect.y + rect.h,			0, 0,	color.x, color.y, color.z, color.w };

    std::vector<gui::VertexData> vtxdata;
    std::vector<unsigned int> indices;

    segment = std::max(segment, 0);
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float step = 0.25 * 2.0 * pi / segment;

    int idx = 0; bool ok = false;
    float x = rect.x + rect.w - r, y = rect.y + rect.h - r;

    // float ux = r / rect.w;
    // float uy = r / rect.h;

    // Bottom-right arcs
    for(float d = 0; !ok; d += step)
    {
	if(d > 0.5 * pi)
	{
	    d = 0.5 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	// float u  = cosf(d) * ux - ux + 1;
	// float v  = uy - sinf(d) * uy;
	// vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	vtxdata.push_back({ vx, vy, 0, 0, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Bottom-left arcs
    x = rect.x + r, y = rect.y + rect.h - r;
    for(float d = 0.5 * pi, ok = false; !ok; d += step)
    {
	if(d > pi)
	{
	    d = pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	// float u  = ux - cosf(pi - d) * ux;
	// float v  = uy - sinf(pi - d) * uy;
	// vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	vtxdata.push_back({ vx, vy, 0, 0, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Top-left arcs
    x = rect.x + r, y = rect.y + r;
    for(float d = pi, ok = false; !ok; d += step)
    {
	if(d > 1.5 * pi)
	{
	    d = 1.5 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	// float u  = ux - cosf(d - pi) * ux;
	// float v  = sinf(d - pi) * uy - uy + 1;
	// vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	vtxdata.push_back({ vx, vy, 0, 0, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Top-right arcs
    x = rect.x + rect.w - r, y = rect.y + r;
    for(float d = 1.5 * pi, ok = false; !ok; d += step)
    {
	if(d > 2 * pi)
	{
	    d = 2 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	// float u  = sinf(d - 1.5 * pi) * ux - ux + 1;
	// float v  = cosf(d - 1.5 * pi) * uy - uy + 1;
	// vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	vtxdata.push_back({ vx, vy, 0, 0, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::RoundRect, vtxdata, indices);
    cmd->lineSize = lineSize;
    return cmd;
}

gui::DrawCommand* GLRenderer::makeFillRoundRect(const Rect& rect, float r, math::vec4 color, GLTexture *texture, int segment)
{
    r = r > std::min(rect.w / 2.0f, rect.h / 2.0f) ? std::min(rect.w / 2.0f, rect.h / 2.0f) : r;

    std::vector<gui::VertexData> vtxdata;
    std::vector<unsigned int> indices;

    segment = std::max(segment, 0);
    float pi = 3.14159265358979323846264338327950288419716939937510;
    float step = 0.25 * 2.0 * pi / segment;

    int idx = 0; bool flip = true, ok = false;
    float x = rect.x + rect.w - r, y = rect.y + rect.h - r;

    float ux = r / rect.w;
    float uy = r / rect.h;

    vtxdata.push_back({ rect.x + rect.w / 2, rect.y + rect.h / 2, .5, .5, color.x, color.y, color.z, color.w });
    indices.push_back(idx++);

    // Bottom-right arcs
    for(float d = 0; !ok; d += step)
    {
	if(d > 0.5 * pi)
	{
	    d = 0.5 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	float u  = cosf(d) * ux - ux + 1;
	float v  = uy - sinf(d) * uy;
	vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Bottom-left arcs
    x = rect.x + r, y = rect.y + rect.h - r;
    for(float d = 0.5 * pi, ok = false; !ok; d += step)
    {
	if(d > pi)
	{
	    d = pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	float u  = ux - cosf(pi - d) * ux;
	float v  = uy - sinf(pi - d) * uy;
	vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Top-left arcs
    x = rect.x + r, y = rect.y + r;
    for(float d = pi, ok = false; !ok; d += step)
    {
	if(d > 1.5 * pi)
	{
	    d = 1.5 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	float u  = ux - cosf(d - pi) * ux;
	float v  = sinf(d - pi) * uy - uy + 1;
	vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // Top-right arcs
    x = rect.x + rect.w - r, y = rect.y + r;
    for(float d = 1.5 * pi, ok = false; !ok; d += step)
    {
	if(d > 2 * pi)
	{
	    d = 2 * pi;
	    ok = true;
	}
	float vx = r * cosf(d) + x;
	float vy = r * sinf(d) + y;
	float u  = sinf(d - 1.5 * pi) * ux - ux + 1;
	float v  = cosf(d - 1.5 * pi) * uy - uy + 1;
	vtxdata.push_back({ vx, vy, u, flip ? 1 - v : v, color.x, color.y, color.z, color.w });
	indices.push_back(idx++);
    }

    // vtxdata.push_back(vtxdata[1]);
    vtxdata.push_back({ rect.x + rect.w, rect.y + rect.h - r, 1, flip ? 1 - uy : uy, color.x, color.y, color.z, color.w });
    indices.push_back(idx++);

    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::FillRoundRect, vtxdata, indices);
    if(texture) cmd->setTexture(texture);
    return cmd;
}

gui::DrawCommand* GLRenderer::makeCharQuad(PackedCharactor& pc, math::vec4 color, Rect *clip)
{
    gui::VertexData vtxdata[] =
        {
         { pc.x0, pc.y0,	pc.s0, pc.t0,	color.x, color.y, color.z, color.w },
         { pc.x1, pc.y0,	pc.s1, pc.t0,	color.x, color.y, color.z, color.w },
         { pc.x1, pc.y1,	pc.s1, pc.t1,	color.x, color.y, color.z, color.w },
         { pc.x0, pc.y1,	pc.s0, pc.t1,	color.x, color.y, color.z, color.w },
        };
    unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };
    gui::DrawCommand *cmd = new gui::DrawCommand(DrawType::Text);
    cmd->setVtxData(vtxdata, sizeof(vtxdata) / sizeof(gui::VertexData));
    cmd->setIdxData(indices, sizeof(indices) / sizeof(unsigned int));
    cmd->setTexture(pc.bw, pc.bh, pc.bimap, Alpha);
    if(clip) cmd->clip = clip;
    return cmd;
}


void GLRenderer::drawRectangle(const Rect& rect, math::vec4 color, float lineSize)
{
    this->draw(this->makeRectangle(rect, color, lineSize));
}

void GLRenderer::drawFillRect(const Rect& rect, math::vec4 color, GLTexture *texture)
{
    this->draw(this->makeFillRect(rect, color, texture));
}

void GLRenderer::drawCircle(float x, float y, float r, math::vec4 color, float lineSize, int segment)
{
    this->draw(this->makeCircle(x, y, r, color, lineSize, segment));
}

void GLRenderer::drawFillCircle(float x, float y, float r, math::vec4 color, GLTexture *texture, int segment)
{
    this->draw(makeFillCircle(x, y, r, color, texture, segment));
}

void GLRenderer::drawRoundRect(const Rect& rect, float r, math::vec4 color, float lineSize, int segment)
{
    this->draw(this->makeRoundRect(rect, r, color, lineSize, segment));
}

void GLRenderer::drawFillRoundRect(const Rect& rect, float r, math::vec4 color, GLTexture *texture, int segment)
{
    this->draw(this->makeFillRoundRect(rect, r, color, texture, segment));
}

GLRenderer::~GLRenderer()
{

}


//
// Static member initialize
//
const char *GLRenderer::ui_vs = (char *)R"glsl(
	#version 330 core
 	layout(location = 0) in vec2 position;
 	layout(location = 1) in vec2 uv;
 	layout(location = 2) in vec4 color;
	uniform mat4 projection;
	out vec2 UV;
	out vec4 Color;
	void main()
	{
	    UV = uv;
	    Color = color;
	    gl_Position = projection * vec4(position, 0.0, 1.0);
	}
	)glsl";

const char *GLRenderer::ui_fs = (char *)R"glsl(
    	#version 330 core
	in vec2 UV;
	in vec4 Color;
	uniform vec4 color;
	uniform float factor;
    	out vec4 FragColor;
    	void main()
    	{
 	    FragColor = Color * color * factor;
    	}
    	)glsl";


// const char *GLRenderer::ui_text_vs = GLRenderer::ui_vs;
const char *GLRenderer::ui_text_fs = (char *)R"glsl(
    	#version 330 core
	in vec2 UV;
	in vec4 Color;
	uniform vec4 color;
	uniform float factor;
	uniform sampler2D image;
    	out vec4 FragColor;
    	void main()
    	{
	    FragColor = texture(image, UV.st).r * Color * color * factor;
	    // FragColor = texture(image, UV.st).r * color * factor;
    	}
    	)glsl";

// const char *GLRenderer::ui_texture_vs = GLRenderer::ui_vs;
const char *GLRenderer::ui_texture_fs = (char *)R"glsl(
    	#version 330 core
	in vec2 UV;
	in vec4 Color;
	uniform vec4 color;
	uniform float factor;
	uniform sampler2D image;
    	out vec4 FragColor;
    	void main()
    	{
	    FragColor = Color * color * factor * texture(image, UV.st);
	    // FragColor = texture(image, UV.st);
    	}
    	)glsl";
