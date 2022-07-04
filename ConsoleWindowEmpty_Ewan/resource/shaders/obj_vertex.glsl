#version 400

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 normal;
layout(location = 2) in vec2 uvCoord;

smooth out vec4 vertPos;
smooth out vec4 vertNormal;
smooth out vec2 vertUV;

uniform mat4 ProjectionViewMatrix;
uniform mat4 ModelMatrix;

void main()
{	
	vertUV = uvCoord;
	vertNormal = normal;
	vertPos = ModelMatrix * position; //World space position
	gl_Position = ProjectionViewMatrix * ModelMatrix * position;	//Screen space position
}