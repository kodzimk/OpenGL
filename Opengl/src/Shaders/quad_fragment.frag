#fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D tex_u;

void main()
{             

    FragColor = texture(tex_u,TexCoords);

}