#include<GL/glew.h>
#include <GLFW/glfw3.h>
#include"imgui.h"
#include"imgui_impl_glfw.h"
#include"imgui_impl_opengl3.h"
#include<vector>
#include<string>
#include<math.h>
#include<fstream>
#include<iostream>
#include<sstream>
#include<cmath>


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>

struct ShaderProgramSource {
    std::string VertexSource;
    std::string FragmentSource;
};
static ShaderProgramSource ParseShaders(const std::string& filepath, const std::string& filepathFrag)
{
    std::ifstream stream(filepath);


    enum class ShaderType {
        NONE = -1, VERTEX = 0, FRAGMENT = 1
    };

    std::string line;
    std::stringstream ss[2];
    ShaderType type = ShaderType::NONE;

    while (getline(stream, line))
    {
        if (line.find("vertex") != std::string::npos)
        {
            type = ShaderType::VERTEX;

        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    stream.close();

    stream.open(filepathFrag);

    while (getline(stream, line))
    {
        if (line.find("#fragment") != std::string::npos)
        {
            type = ShaderType::FRAGMENT;

        }
        else
        {
            ss[(int)type] << line << '\n';
        }
    }

    const char* vertexS = ss[0].str().c_str();

    const char* vertexSS = ss[1].str().c_str();

    return { ss[0].str(),ss[1].str() };

}


#define SRC_WIDTH 800
#define SRC_HEIGHT 600

float vertices1[] = {
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
       -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
       -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
       -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f ,0.0f, 1.0f
};

unsigned int CreateProgram(const char* vertexSrc, const char* fragmentSrc)
{
    unsigned int program = glCreateProgram();
    
    ShaderProgramSource src = ParseShaders(vertexSrc, fragmentSrc);

    const GLchar* vertex = src.VertexSource.c_str();
    const GLchar* fragment = src.FragmentSource.c_str();

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex, NULL);
    glCompileShader(vertexShader);


    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment, NULL);
    glCompileShader(fragmentShader);


    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    return program;
}
 
float quadVertices[] = {
-1.0f,  1.0f,  0.0f, 1.0f,
-1.0f, -1.0f,  0.0f, 0.0f,
 1.0f, -1.0f,  1.0f, 0.0f,

-1.0f,  1.0f,  0.0f, 1.0f,
 1.0f, -1.0f,  1.0f, 0.0f,
 1.0f,  1.0f,  1.0f, 1.0f
};


float quad[] = {
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
        0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       -0.5f,  0.5f, -0.5f, 0.0f, 1.0f,
       -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

       - 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
        0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
       - 0.5f,  0.5f,  0.5f, 0.0f, 1.0f,
       - 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,

       - 0.5f,  0.5f,  0.5f, 1.0f, 0.0f,
       - 0.5f,  0.5f, -0.5f, 1.0f, 1.0f,
       - 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       - 0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
       - 0.5f, -0.5f,  0.5f, 0.0f, 0.0f,
       - 0.5f,  0.5f,  0.5f,1.0f, 0.0f,

        0.5f,  0.5f,  0.5f,1.0f, 0.0f,
        0.5f,  0.5f, -0.5f,1.0f, 1.0f,
        0.5f, -0.5f, -0.5f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,0.0f, 1.0f,
        0.5f, -0.5f,  0.5f,0.0f, 0.0f,
        0.5f,  0.5f,  0.5f,1.0f, 0.0f,

       -0.5f, -0.5f, -0.5f,0.0f, 1.0f,
        0.5f, -0.5f, -0.5f,1.0f, 1.0f,
        0.5f, -0.5f,  0.5f,1.0f, 0.0f,
        0.5f, -0.5f,  0.5f,1.0f, 0.0f,
       -0.5f, -0.5f,  0.5f,0.0f, 0.0f,
       -0.5f, -0.5f, -0.5f,0.0f, 1.0f,

       -0.5f,  0.5f, -0.5f,0.0f, 1.0f,
        0.5f,  0.5f, -0.5f,1.0f, 1.0f,
        0.5f,  0.5f,  0.5f,1.0f, 0.0f,
        0.5f,  0.5f,  0.5f,1.0f, 0.0f,
       -0.5f,  0.5f,  0.5f,0.0f, 0.0f,
       -0.5f,  0.5f, -0.5f,0.0f, 1.0f
};


enum STATE {
    IDLE = 0,
    WALK = 1,
    RUN = 2,
    ATTACK = 3
};




std::vector<glm::mat4> matrixs;

struct Box {
    float front, back, top, bottom, left, right;
};

bool boxCollision(Box box1, Box box2) {
    bool xCollision = box1.right >= box2.left && box1.left <= box2.right;
    bool yCollision = box1.top >= box2.bottom && box1.bottom <= box2.top;
    bool zCollision = box1.front >= box2.back && box1.back <= box2.front;

    return xCollision && yCollision && zCollision;
}

bool collision(glm::vec3 playerPosition,glm::vec3 objectPosition,float widthOfPlayer,float heightOfPlayer, float depthOfPlayer,
    float widthofobject,float heightofobject,float depthOfObject)
{
    Box box1;
    Box box2;
    
    box1.top = playerPosition.y + heightOfPlayer / 2;
    box1.bottom = playerPosition.y - heightOfPlayer / 2;
    box1.front = playerPosition.z + depthOfPlayer / 2;
    box1.back = playerPosition.z - depthOfPlayer / 2;
    box1.left = playerPosition.x - widthOfPlayer / 2;
    box1.right = playerPosition.x + widthOfPlayer / 2;

    box2.top = objectPosition.y + heightofobject / 2;
    box2.bottom = objectPosition.y - heightofobject / 2;
    box2.front = objectPosition.z + depthOfObject / 2;
    box2.back = objectPosition.z - depthOfObject / 2;
    box2.left = objectPosition.x - widthofobject / 2;
    box2.right = objectPosition.x + widthofobject / 2;

   return boxCollision(box1, box2);
}
