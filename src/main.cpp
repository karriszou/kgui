#include <iostream>
#include <cmath>
#include <vector>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Vector.h"
#include "Matrix.h"
#include "Rect.h"

#include "StbFont.h"
#include "GLRenderer.h"
#include "GUI.h"
#include "Label.h"
#include "Text.h"
#include "Button.h"
#include "Slider.h"
#include "Progressbar.h"
#include "Checkbox.h"
#include "Radio.h"


#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

using namespace std;

#define WIDTH  1024
#define HEIGHT 768

GLFWwindow *window;

struct Camera
{
    math::vec3 position;
    float yaw, pitch;

    math::vec3 forward;
    math::vec3 up;
    math::vec3 right;
    float fov, aspect, near, far;

    Camera()
	:position(0, 1, 3), yaw(-89.5), pitch(0),
	 forward(0, 0, -1), up(0, 1, 0), right(1, 0, 0),
	 fov(45.0), aspect(4.0 / 3.0), near(0.1), far(100.0)
    {
	view.lookAt(forward, up).translate(-position);
	projection.perspective(fov, aspect, near, far);
    }

    math::Mat4& getView()
    {
    	// return this->view.identity().lookAt(forward, up).translate(-position);
	return this->view.identity().lookAt(-position).translate(-position);
    }

    math::Mat4& getPerspective()
    {
	return this->projection.perspective(fov, aspect, near, far);
    }

    math::Mat4& getPerspective(float fov, float aspect, float near, float far)
    {
	return this->projection.perspective(fov, aspect, near, far);
    }

    math::Mat4& getOrtho()
    {
	float s = 1.0;
	if(WIDTH > HEIGHT)
	    return this->projection.ortho(-s * WIDTH / HEIGHT, s * WIDTH / HEIGHT, -s, s, -s * near, s * far);
	else
	    return this->projection.ortho(-s, s, -s * WIDTH / HEIGHT, s * WIDTH / HEIGHT, -s * near, s * far);
    }

private:
    math::Mat4 view;
    math::Mat4 projection;
};

Camera camera;

bool createWindow()
{
    if(!glfwInit())
    {
	cerr << "Can not init glfw!" << endl;
	return false;
    }
    // glfwWindowHint(GLFW_CENTER_CURSOR, GLFW_TRUE);
    window = glfwCreateWindow(WIDTH, HEIGHT, "Game GUI", NULL, NULL);
    if(!window)
    {
	cerr << "Can not create window!" << endl;
	return false;
    }
    const GLFWvidmode *vm = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if(vm) glfwSetWindowPos(window, (vm->width - WIDTH) / 2, (vm->height - HEIGHT) / 2);
    glfwSetCursorPos(window, WIDTH / 2.0, HEIGHT / 2.0);
    glfwPollEvents();
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    return true;
}

GLuint getShader(const char *vs_code, const char *fs_code)
{
    GLint status; GLchar info[512];
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_code, NULL);
    glCompileShader(vs);
    glGetShaderiv(vs, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
	glGetShaderInfoLog(vs, sizeof(info), NULL, info);
	cerr << "Error: Vertex shader can't compile: \n" << info << endl;
	return -1;
    }
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_code, NULL);
    glCompileShader(fs);
    glGetShaderiv(fs, GL_COMPILE_STATUS, &status);
    if(status != GL_TRUE)
    {
	glGetShaderInfoLog(fs, sizeof(info), NULL, info);
	cerr << "Error: Fragment shader can't compile: \n" << info << endl;
	return -1;
    }
    GLuint shader = glCreateProgram();
    glAttachShader(shader, vs);
    glAttachShader(shader, fs);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &status);
    if(status != GL_TRUE)
    {
	glGetProgramInfoLog(shader, sizeof(info), NULL, info);
	cerr << "Error: Shader program can't link: \n" << info << endl;
	return -1;
    }
    return shader;
}

GLuint makeCube(GLuint *shader)
{
    GLfloat data[] =
	{
	 // front
	 -1, -1, -1,
	  1, -1, -1,
	 -1,  1, -1,
	  1, -1, -1,
	  1,  1, -1,
	 -1,  1, -1,
	 // back
	 -1, -1, 1,
	 -1,  1, 1,
	  1, -1, 1,
	  1, -1, 1,
	 -1,  1, 1,
	  1,  1, 1,
	 // left
	 -1, -1, -1,
	 -1,  1, -1,
	 -1, -1,  1,
	 -1, -1,  1,
	 -1,  1, -1,
	 -1,  1,  1,
	 // right
	  1, -1, -1,
	  1, -1,  1,
	  1,  1, -1,
	  1, -1,  1,
	  1,  1,  1,
	  1,  1, -1,
	 // top
	 -1,  1,  1,
	 -1,  1, -1,
	  1,  1,  1,
	  1,  1,  1,
	 -1,  1, -1,
	  1,  1, -1,
	 // bottom
	 -1, -1,  1,
	  1, -1,  1,
	 -1, -1, -1,
	  1, -1,  1,
	  1, -1, -1,
	 -1, -1, -1,
	};
    GLuint vao, vbo;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    const char *vs_code = R"glsl(
	#version 330 core
 	layout(location = 0) in vec3 position;
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
	uniform bool rgb;
	out vec4 color;
	void main()
	{
	    gl_Position = projection * view * model * vec4(position, 1.0);
	    if(rgb)
	    {
		float s = 0.8;
		int v = gl_VertexID / 6;
		if(v == 0) color = vec4(s, 0, 0, 1);
		if(v == 1) color = vec4(0, s, 0, 1);
		if(v == 2) color = vec4(0, 0, s, 1);
		if(v == 3) color = vec4(s, s, 0, 1);
		if(v == 4) color = vec4(s, 0, s, 1);
		if(v == 5) color = vec4(0, s, s, 1);
	    }
	    else
		color = vec4(1);
	}
	)glsl";

    const char *fs_code = R"glsl(
    	#version 330 core
	in vec4 color;
    	out vec4 FragColor;
    	void main()
    	{
	    FragColor = color;
    	}
    	)glsl";

    *shader =  getShader(vs_code, fs_code);
    return vao;
}

float speed = 50;
bool pause = false;
bool rgb = false, vertical = false;
void renderCube(GLuint vao, GLuint shader)
{
    glViewport(0, 0, WIDTH, HEIGHT);
    glEnable(GL_DEPTH_TEST);    

    static math::mat4 model = (*new math::mat4()).scale(.5);
    if(!pause)
    	model.rotate(speed * 3e-4, math::vec3::up());
    if(!pause && vertical)
	model.rotate(speed * 3e-4, math::vec3::right());

    int w, h;
    glfwGetFramebufferSize(window, &w, &h);
    glViewport(0, 0, w, h);

    glUseProgram(shader);
    glUniform1i(glGetUniformLocation(shader, "rgb"), rgb);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, model.get());
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, camera.getView().get());
    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE,
		       camera.getPerspective(45.0, (float)w / h, 0.1, 100.0).get());
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

void onKey(GLFWwindow *win, int key, int scancode, int action, int mods)
{
    float dt = 0.02; (void)win; (void)scancode; (void)mods;
    if(action == GLFW_PRESS || action == GLFW_REPEAT)
    {
	float m = 3.0 * dt;
	if(key == GLFW_KEY_W)
	    camera.position +=  camera.forward * m;
	if(key == GLFW_KEY_S)
	    camera.position += -camera.forward * m;
	if(key == GLFW_KEY_D)
	    camera.position +=  camera.right * m;
	if(key == GLFW_KEY_A)
	    camera.position += -camera.right * m;
	if(key == GLFW_KEY_SPACE)
	    camera.position +=  camera.up * m;
	if(key == GLFW_KEY_TAB)
	    camera.position += -camera.up * m;
    }
    // cout << camera.x << ", " << camera.y << ", " << camera.z << endl;
}

void onCursor(GLFWwindow *win, double xpos, double ypos)
{
    return;

    static float px, py; (void)win;
    static float yaw = -89.5, pitch;
    if(px != 0 || py != 0)
    {
	float dx = xpos - px;
	float dy = py - ypos;
	float s  = 0.1;
	dx *= s; dy *= s;
	yaw   += dx * 3.141592653 / 180.0;
	pitch += dy * 3.141592653 / 180.0;
	pitch = pitch > 89.0 ? 89.0 : (pitch < -89.0 ? -89.0 : pitch);
	// cout << "\t\t\tdx: " << dx << "\tdy: " << dy
	//      << "\tyaw: " << yaw << "\tpitch: " << pitch << endl;
	camera.forward.x = cosf(yaw) * cosf(pitch);
	camera.forward.y = sinf(pitch);
	camera.forward.z = sinf(yaw) * cosf(pitch);
	camera.forward.normalize();
	// compute side(right) vector
	math::vec3 world_up(0, 1, 0);
	camera.right = camera.forward.cross(world_up).normalize();
	// re-compute up vector
	camera.up = camera.right.cross(camera.forward).normalize();
    }
    px = xpos; py = ypos;
}

void onMouse(GLFWwindow *win, int button, int action, int mods)
{
    (void)win; (void)mods;
    if(action == GLFW_PRESS)
    {
	if(button == GLFW_MOUSE_BUTTON_LEFT)
	    pause = !pause;
	std::cout << "Pause" << std::endl;
    }
    else
    {
	
    }
}

int main()
{
    if(!createWindow())
    {
	cerr << "Could not create window!" << endl;
	return 1;
    }
    // glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(window, onKey);
    glfwSetCursorPosCallback(window, onCursor);
    // glfwSetMouseButtonCallback(window, onMouse);
    if(!gladLoadGL())
    {
	cerr << "Could not load OpenGL!" << endl;
	return 1;
    }
    glViewport(0, 0, WIDTH, HEIGHT);
    glClearColor(.2, .2, .2, 1);

    // int tw, th, chanel;
    // unsigned char *tex_data = stbi_load("../shared/button.png", &tw, &th, &chanel, 0);
    // GLTexture *tex = new GLTexture(tw, th, tex_data);

    StbFont font1("../shared/droidsans.ttf");
    StbFont font2("../shared/proggyclean.ttf");
    StbFont font_han("../shared/hiraginogb.ttf");

    // GLRenderer renderer(0, 0, WIDTH, HEIGHT);
    // gui::Label label(Rect(700, 600, 80, 30), "Label", font2);

    gui::Container panel;
    panel.layout(0, 1000, .3f, .5f, gui::Center);


    gui::Text text("60", font1);
    text.fgColor = math::vec4(1);
    text.bgColor = math::vec4(.1, .1, .1, .6);
    text.layout(0, 0, .08f, .04f, gui::TopRight);
    
    gui::Button button("Pause", font2);
    button.layout(.0f, .05f, .5f, .08f, gui::Top, &panel);
    button.radius  = 12;
    button.fgColor = math::vec4(1);
    button.bgColor = math::vec4(.1, .1, .1, .6);
    button.onClick = [](void *sender, gui::GUIEven e)
    		     {
			 (void)sender; (void)e;
			 pause = !pause;
    		     };

    gui::Slider slider(20, 100);
    slider.layout(.0f, .18f, .8f, .05f, gui::Top, &panel);
    slider.onChanged = [](void *sender, float v)
		       {
			   (void)sender; speed = v;
		       };
    
    gui::Progressbar pb(10, 30);
    pb.layout(.0f, .28f, .8f, .05f, gui::Top, &panel);

    gui::Checkbox check_rgb("RGB", font2);
    check_rgb.layout(-.2f, 0.38f, .4f, .08f, gui::Top, &panel);
    check_rgb.check();
    gui::Checkbox check_vtc("Vertical", font2);
    check_vtc.layout( .2f, 0.38f, .4f, .08f, gui::Top, &panel);
    check_vtc.onCheck = [](void *sender, bool value)
    			{
    			    (void)sender; vertical = value;
    			};

    gui::Radio radio_r("R", font2);
    radio_r.layout(-.3f, .5f, .2f, .08f, gui::Top, &panel);
    radio_r.check();

    gui::Radio radio_g("G", font2);
    radio_g.layout( .0f, .5f, .2f, .08f, gui::Top, &panel);

    gui::Radio radio_b("B", font2);
    radio_b.layout( .3f, .5f, .2f, .08f, gui::Top, &panel);


    GLuint shader_cube;
    GLuint vao_cube = makeCube(&shader_cube);

    while(!glfwWindowShouldClose(window))
    {
	if(glfwGetKey(window, GLFW_KEY_ESCAPE))
	    glfwSetWindowShouldClose(window, GLFW_TRUE);
	static float last = 0.0;
	float time = glfwGetTime();
	float dt = time - last; last = time;

	int w, h;
	glfwGetFramebufferSize(window, &w, &h);
	glViewport(0, 0, w, h);
	glScissor(0, 0, w, h);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	double cx, cy;
	glfwGetCursorPos(window, &cx, &cy);
	bool left  = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
	bool right = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT);

	// Draw scene
	renderCube(vao_cube, shader_cube);

	// Draw GUI
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	static float interal = 0;
	interal += dt;
	if(interal > 1)
	{
	    text.text = std::to_wstring((int)(1 / dt)).substr(0, 2);
	    interal = 0;
	    pb = dt * 1000;
	}

	static int py = 1000;
	if(py > 0)
	{
	    py -= dt * 800;
	    panel.layout(0, py, .3f, .5f, gui::Center);
	}

	if(glfwGetKey(window, GLFW_KEY_LEFT))
	    slider -= 20 * dt;
	if(glfwGetKey(window, GLFW_KEY_RIGHT))
	    slider += 20 * dt;

	rgb = check_rgb;

	gui::GUI::update(w, h, cx, cy, 0, 0, left, right);
	gui::GUI::render();

	// renderer.draw(cmd);
	// float ortho[4][4] =
	//     {{  2,  0,  0, 0 },
	//      {  0, -2,  0, 0 },
	//      {  0,  0, -1, 0 },
	//      { -1,  1,  0, 1 }};
	// ortho[0][0] /= w;
	// ortho[1][1] /= h;
	// glUseProgram(shader);
	// glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &ortho[0][0]);
	// glBindVertexArray(vao);
	// glDrawElements(GL_LINE_LOOP, indices.size(), GL_UNSIGNED_INT, &indices[0]);

	glfwPollEvents();
	glfwSwapBuffers(window);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
