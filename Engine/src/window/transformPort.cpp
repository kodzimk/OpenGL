#include "transformPort.h"
#include<GLFW/glfw3.h>
#include<string>

transformPort::transformPort(GLFWwindow* window)
{
    cntx = ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();
    ioRef = &io;
}

transformPort::~transformPort()
{
}

void transformPort::render(GLFWwindow* window)
{
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        ImGui::Begin("Components", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_None);

        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(400,700));
        ImGui::SetWindowCollapsed(false);



        if (ImGui::Button("on face cull", ImVec2(100, 100)))
        {
            glEnable(GL_CULL_FACE);
            glCullFace(GL_FRONT_FACE);
            glFrontFace(GL_CCW);
        }

        if (ImGui::Button("off face cull", ImVec2(100, 100)))
        {
            glDisable(GL_CULL_FACE);
        }

        double x = 0, y = 0;
        glfwGetCursorPos(window, &x, &y);

        std::string xPos = "X: " + std::to_string(static_cast<int>(x));
        std::string yPos = "Y: " + std::to_string(static_cast<int>(y));
        std::string fpsC = std::to_string(fps);


        ImGui::Text(xPos.c_str());
        ImGui::Text(yPos.c_str());
        ImGui::Text(fpsC.c_str());

        ImGui::End();
        ImGui::Render();
        ImGui::EndFrame();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}
