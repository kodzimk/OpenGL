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


    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT_FACE);
    glFrontFace(GL_CCW);

    ImVec2 windowedModeRes = ImVec2(mode->width, mode->height);
    glm::vec3 angles = glm::vec3(0.0f, 0.0f, 0.0f);

    transformPort port(window);
    ImGui::SetCurrentContext(port.cntx);

        while (!glfwWindowShouldClose(window))
        {
         

            camera.Inputs(window,*port.ioRef);
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
            port.render(window);
          
           
    
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

    glfwTerminate();
    return 0;
}
