#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"

class transformPort
{
public:

	transformPort(GLFWwindow* window);
	~transformPort();

public:
	void render(GLFWwindow* window);

public:
	ImGuiContext* cntx;
	ImGuiIO* ioRef;
	int fps;


};

