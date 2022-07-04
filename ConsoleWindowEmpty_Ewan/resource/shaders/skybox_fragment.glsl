#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube CubeMap;

void main()
{
    FragColor = texture(CubeMap, TexCoords);
}