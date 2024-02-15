#vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 TexCoord;
layout(location = 2) in vec3 aNormal;

out vec2 TexCoords;
out vec3 crnt;
out vec3 Normal;

uniform mat4 playerMatrix;
uniform mat4 model;

void main()
{
  TexCoords = TexCoord;
  Normal = aNormal;
  crnt = vec3(model*vec4(aPos,1.0f));
    gl_Position = playerMatrix * model * vec4(aPos, 1.0);
}