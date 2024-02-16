#fragment
#version 330 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 crnt;

uniform sampler2D diffuseTexture;
uniform vec3 lightPos;
uniform vec3 viewPos;



void main()
{           
    float ambient = 0.20f;
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crnt);
    float diffuse = max(dot(normal,lightDirection),0.0);

    float specular = 0.0f;

    if(diffuse != 0.0f)
    {
        float specPower = 0.5f;
        vec3 viewDirection = normalize(viewPos - crnt);
        vec3 halfWayVec = normalize(viewDirection+lightDirection);
        float specAmount = pow(max(dot(normal,halfWayVec),0.0),16);
        specular = specAmount * specPower;
    }

    FragColor.rgb = texture(diffuseTexture,TexCoords).rgb*(diffuse+specular+ambient);
}