#ifndef GL_RENDERER_H
#define GL_RENDERER_H

#include <iostream>
#include <cstring>
#include <vector>

#include "glad.h"
#include "Rect.h"
#include "StbFont.h"


class GLShader;
struct GLTexture;

namespace gui
{
    struct VertexData;
    struct DrawCommand;
    class IDrawable;
}

enum DrawType : int
    {
     Point = 0,
     Line,
     Triangle,
     Rectangle,
     RoundRect,
     Circle,
     Ellipse,
     FillTriangle,
     FillRect,
     FillRoundRect,
     FillCircle,
     FillEllipse,
     Text
    };

class GLRenderer
{
public:
    static GLShader *UIShader;
    static GLShader *TextShader;
    static GLShader *TextureShader;
    static const char *ui_vs;
    static const char *ui_fs;
    static const char *ui_text_fs;
    static const char *ui_texture_fs;

    float x, y, w, h;

    GLRenderer(float x, float y, float w, float h);

    ~GLRenderer();

    void viewPort(float x, float y, float w, float h)
    {
	this->x = x; this->y = y;
	this->w = w; this->h = h;
    }

    void draw(const gui::DrawCommand* cmd);

    void draw(gui::IDrawable& drawable);


    static gui::DrawCommand* makeRectangle(const Rect& rect, math::vec4 color = math::vec4(1), float lineSize = 1);

    static gui::DrawCommand* makeFillRect(const Rect& rect, math::vec4 color = math::vec4(1), GLTexture *texture = NULL);

    static gui::DrawCommand* makeCircle(float x, float y, float r, math::vec4 color = math::vec4(1), float lineSize = 1, int segment = 28);

    static gui::DrawCommand* makeFillCircle(float x, float y, float r, math::vec4 color = math::vec4(1), GLTexture *texture = NULL, int segment = 28);

    static gui::DrawCommand* makeRoundRect(const Rect& rect, float r, math::vec4 color = math::vec4(1), float lineSize = 1, int segment = 8);

    static gui::DrawCommand* makeFillRoundRect(const Rect& rect, float r, math::vec4 color = math::vec4(1), GLTexture *texture = NULL, int segment = 8);

    static gui::DrawCommand* makeCharQuad(PackedCharactor& pc, math::vec4 color = math::vec4(1), Rect *clip = NULL);



    void drawRectangle(const Rect& rect, math::vec4 color = math::vec4(1), float lineSize = 1);

    void drawFillRect(const Rect& rect, math::vec4 color = math::vec4(1), GLTexture *texture = NULL);

    void drawCircle(float x, float y, float r, math::vec4 color = math::vec4(1), float lineSize = 1, int segment = 28);

    void drawFillCircle(float x, float y, float r, math::vec4 color = math::vec4(1), GLTexture *texture = NULL, int segment = 28);

    void drawRoundRect(const Rect& rect, float r, math::vec4 color = math::vec4(1), float lineSize = 1, int segment = 8);

    void drawFillRoundRect(const Rect& rect, float r, math::vec4 color = math::vec4(1), GLTexture *texture = NULL,int segment = 8);

private:
    GLuint vao, vbo;

};


//
// Misc classes
//
class GLShader
{
public:

    // Load shader from memory
    GLShader(const char *vsCode, const char *fsCode)
    {
	GLint status; GLchar info[512];
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vsCode, NULL);
	glCompileShader(vs);
	glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
	    glGetShaderInfoLog(vs, sizeof(info), NULL, info);
	    std::cerr << "Error: Vertex shader can't compile: \n" << info << std::endl;
	}
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fsCode, NULL);
	glCompileShader(fs);
	glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
	if(status != GL_TRUE)
	{
	    glGetShaderInfoLog(fs, sizeof(info), NULL, info);
	    std::cerr << "Error: Fragment shader can't compile: \n" << info << std::endl;
	}
	GLuint shader = glCreateProgram();
	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glGetProgramiv(shader, GL_LINK_STATUS, &status);
	if(status != GL_TRUE)
	{
	    glGetProgramInfoLog(shader, sizeof(info), NULL, info);
	    std::cerr << "Error: Shader program can't link: \n" << info << std::endl;
	}

	glDeleteShader(vs);
	glDeleteShader(fs);
	this->id = shader;
    }

    unsigned int getId()
    {
	return this->id;
    }

    void use(bool enable = true)
    {
	enable ? glUseProgram(this->id) : glUseProgram(0);
    }

    void setInt(const char *name, int value)
    {
	glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void setFloat(const char *name, float value)
    {
	glUniform1f(glGetUniformLocation(this->id, name), value);
    }

    void setBool(const char *name, bool value)
    {
	glUniform1i(glGetUniformLocation(this->id, name), value);
    }

    void setVec3(const char *name, float value[3])
    {
	glUniform3fv(glGetUniformLocation(this->id, name), 1, value);
    }

    void setVec4(const char *name, float value[4])
    {
	glUniform4fv(glGetUniformLocation(this->id, name), 1, value);
    }

    void setMat4(const char *name, float value[4][4])
    {
	glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, GL_FALSE, &value[0][0]);
    }
    

private:
    unsigned int id;
};

enum GLTextureFormat : int
    {
     Rgba = 0,
     Rgb,
     Alpha
    };

struct GLTexture
{
public:
    unsigned int id;

    GLTexture(int w, int h, unsigned char * data, GLTextureFormat format = Rgba)
    {
    	glGenTextures(1, &this->id);
    	glBindTexture(GL_TEXTURE_2D, this->id);
    	switch(format)
    	{
    	case Rgb:
    	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    	    break;
    	case Alpha:
	    // glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, w, h, 0, GL_ALPHA, GL_UNSIGNED_BYTE, data);
	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, w, h, 0, GL_RED, GL_UNSIGNED_BYTE, data);
    	    break;
    	default:
    	case Rgba:
    	    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    	    break;
    	}
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBindTexture(GL_TEXTURE_2D, 0);
    }

    ~GLTexture()
    {
	glDeleteTextures(1, &this->id);
    }

};


#endif
