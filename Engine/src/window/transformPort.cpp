#include "transformPort.h"
#include<GLFW/glfw3.h>

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

        ImGui::Begin("niger", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);

        ImGui::SetWindowPos(ImVec2(0, 0));
        ImGui::SetWindowSize(ImVec2(400,400));
        ImGui::SetWindowCollapsed(false);



        if (ImGui::Button("previous", ImVec2(300, 222)))
        {
           
        }

        ImGui::End();
        ImGui::Render();
        ImGui::EndFrame();

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
}
