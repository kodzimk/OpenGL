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

    window = glfwCreateWindow(SRC_WIDTH, SRC_HEIGHT, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

  
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        return -1;

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices1),vertices1 , GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));
    glEnableVertexAttribArray(1);


    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));
    glEnableVertexAttribArray(2);



    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);


    unsigned int quadVAO, quadVBO;
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 4, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    unsigned int program = CreateProgram("src/Shaders/vertex_shader.vert", "src/Shaders/fragment_shader.frag");
    unsigned int shadowMap = CreateProgram("src/Shaders/shadow_vertex.vert", "src/Shaders/shadow_fragment.frag");
    unsigned int frameBufferProgram = CreateProgram("src/Shaders/framebuffer_vertex.vert", "src/Shaders/framebuffer_fragment.frag");

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

       data = stbi_load(
        "res/texture/container.jpg", &width, &height, &channels, STBI_rgb_alpha);


    unsigned int container;
    glGenTextures(1, &container);

    glBindTexture(GL_TEXTURE_2D, container);


    glTexImage2D(GL_TEXTURE_2D,
        0, GL_RGB, width, height, 0,
        GL_RGBA, GL_UNSIGNED_BYTE, data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);




    glm::mat4 blockMatrix = glm::mat4(1.0f);
    blockMatrix = glm::translate(blockMatrix, glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 quadMatrix = glm::mat4(1.0f);
    quadMatrix = glm::translate(quadMatrix, glm::vec3(0.0f,-1.0f,0.0f));

    glm::mat4 quadMatrix2 = glm::mat4(1.0f);
    quadMatrix2 = glm::translate(quadMatrix, glm::vec3(2.0f, 1.0f, 0.0f));

    Camera camera(SRC_WIDTH,SRC_HEIGHT,glm::vec3(0.0f,2.0f,2.0f),0.0f);

    glUseProgram(program);
    glUniform3f(glGetUniformLocation(program, "lightPos"), 0.0f, 0.0f, -1.0f);
    glUniform1i(glGetUniformLocation(program, "diffuseTexture"), 1);

    glUseProgram(frameBufferProgram);
    glUniform1i(glGetUniformLocation(frameBufferProgram, "screenTexture"), 2);


    unsigned int FBO;
    glGenFramebuffers(1, &FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    unsigned int fboTexture;
    glGenTextures(1, &fboTexture);
    glBindTexture(GL_TEXTURE_2D, fboTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, SRC_WIDTH, SRC_HEIGHT, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture, NULL);

    unsigned int RBO;
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, SRC_WIDTH, SRC_HEIGHT);
    glFramebufferRenderbuffer(GL_TEXTURE_2D, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

    unsigned int depthMapFBO;
    glGenFramebuffers(1, &depthMapFBO);
    
    unsigned int depthMapTexture;
    glGenTextures(1, &depthMapTexture);
    glBindTexture(GL_TEXTURE_2D, depthMapTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
    glReadBuffer(GL_NONE);
    glDrawBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    STATE state = STATE::IDLE;
    Obj_Loader obj;

    obj.make_obj_mesh("res/object/gameloft tasm 2.obj", blockMatrix);

    while (!glfwWindowShouldClose(window))
    {
        camera.Inputs(window);
        camera.updateMatrix(45.f, 0.1f, 100.f);

        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
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

    
        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, container);
        camera.Matrix(program, "playerMatrix");
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(quadMatrix));
        glDrawArrays(GL_TRIANGLES, 0, 110);

        glBindVertexArray(VAO);
        glBindTexture(GL_TEXTURE_2D, container);
        camera.Matrix(program, "playerMatrix");
        glUniformMatrix4fv(glGetUniformLocation(program, "model"), 1, GL_FALSE, glm::value_ptr(quadMatrix2));
        glDrawArrays(GL_TRIANGLES, 0, 110);

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);
        glUseProgram(frameBufferProgram);
        glBindVertexArray(quadVAO);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, fboTexture);
        glDrawArrays(GL_TRIANGLES, 0, 6);
     
        glfwSwapBuffers(window);

        
        glfwPollEvents();
    }

    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1,&VAO);

    glfwTerminate();
    return 0;
}