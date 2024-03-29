#include"external/config.h"
#include"System/Camera.h"
#include"System/Obj_Loader.h"

#define STB_IMAGE_IMPLEMENTATION
#include"external/stb_image.h"


int main(void)
{
    GLFWwindow* window;

    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if (!glfwInit())
        return -1;
 
    const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE); //line added to force decoration but has no effect

    window = glfwCreateWindow(mode->width, mode->height, "Simple example", NULL, nullptr); //create window in fullscreen

    if (!window)
    {
        glfwTerminate();
        return -1;
    }
  
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;

    unsigned int program = CreateProgram("src/Shaders/vertex_shader.vert", "src/Shaders/fragment_shader.frag");

    int width, height, channels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(
        "res/texture/01.png", &width, &height, &channels, STBI_rgb_alpha);


    unsigned int texture1;
    glGenTextures(1, &texture1);

    glBindTexture(GL_TEXTURE_2D, texture1);


    glTexImage2D(GL_TEXTURE_2D,
        0, GL_RGB, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);

    glm::mat4 blockMatrix = glm::mat4(1.0f);
    blockMatrix = glm::translate(blockMatrix, glm::vec3(0.0f, 1.0f, 0.0f));

    Camera camera(SRC_WIDTH,SRC_HEIGHT,glm::vec3(0.0f,2.0f,2.0f),0.0f);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 0.0f, 0.0f, -1.0f);
    glUniform1i(glGetUniformLocation(program, "diffuseTexture"), 1);


    Obj_Loader obj;
    bool my_tool_active = false;
    obj.make_obj_mesh("res/object/gameloft tasm 2.obj", blockMatrix);

    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui::StyleColorsDark();
    ImGui_ImplOpenGL3_Init();

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_FACE);
    glFrontFace(GL_CCW);

    ImVec2 windowedModeRes = ImVec2(mode->width, mode->height);
    glm::vec3 angles = glm::vec3(0.0f, 0.0f, 0.0f);

        while (!glfwWindowShouldClose(window))
        {
         

            camera.Inputs(window,io);
            camera.updateMatrix(45.f, 0.1f, 100.f);

            glEnable(GL_DEPTH_TEST);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);

            glUseProgram(program);
            glBindVertexArray(obj.VAOs[0]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture1);
            glUniform3f(glGetUniformLocation(program, "viewPos"), camera.Position.x, camera.Position.y, camera.Position.z);
            camera.Matrix(program, "playerMatrix");
            glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(blockMatrix));
            glDrawArrays(GL_TRIANGLES, 0, obj.size);

            ImGui_ImplOpenGL3_NewFrame();
            ImGui_ImplGlfw_NewFrame();

            ImGui::NewFrame(); 

            ImGui::Begin("niger",NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
        
            ImGui::SetWindowPos(ImVec2(0, 0));
            ImGui::SetWindowSize(ImVec2(windowedModeRes.x * 0.15, windowedModeRes.y*0.6));
            ImGui::SetWindowCollapsed(false);

            std::string x = "X: " + std::to_string(angles.x);
            std::string y = "Y: " + std::to_string(angles.y);
            std::string z = "Z: " + std::to_string(angles.z);
            ImGui::Text(x.c_str());
            ImGui::Text(y.c_str());
            ImGui::Text(z.c_str());
         
   
            if (ImGui::Button("play", ImVec2(windowedModeRes.x * 0.15 * 0.4, windowedModeRes.y * 0.4 * 0.2)))
            {
                blockMatrix = glm::rotate(blockMatrix, 90.f, glm::vec3(1.0f, 0.0f, 0.0f));
                angles.x += 90.f;
                if (angles.x >= 360.f)
                    angles.x = angles.x - 360.f;
            }

            if (ImGui::Button("previous", ImVec2(windowedModeRes.x * 0.15 * 0.4, windowedModeRes.y * 0.4 * 0.2)))
            {
              glfwSetWindowMonitor(window, nullptr, 400, 300, mode->width*0.5, mode->height * 0.5, GLFW_DONT_CARE);
              windowedModeRes = ImVec2(mode->width * 0.5, mode->height * 0.5);
            }

            ImGui::End();
            ImGui::Render();
            ImGui::EndFrame(); 

            ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
           
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    glfwTerminate();
    return 0;
}
