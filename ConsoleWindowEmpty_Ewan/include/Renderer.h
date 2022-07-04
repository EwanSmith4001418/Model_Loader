#pragma once
#include <imgui.h>
#include "Application.h"
#include "ApplicationEvent.h"
#include <glm/glm.hpp>
#include <string>
//Forward declare OBJ model
class OBJModel;

class Skybox;

class Renderer : public Application
{
public:
	Renderer();
	virtual ~Renderer();

	void onWindowResize(WindowResizeEvent* e);
protected:
	virtual bool onCreate();
	virtual void Update(float deltaTime);
	virtual void Draw();
	virtual void Destroy();

private:
	//Structure for a simple vertex - interleaved (position, colour)
	typedef struct Vertex
	{
		glm::vec4 position;
		glm::vec4 colour;
	}Vertex;

	//Structure for a line
	typedef struct Line
	{
		Vertex v0;
		Vertex v1;
	}Line;

	glm::mat4 m_cameraMatrix;
	glm::mat4 m_projectionMatrix;

	//Shader programs
	unsigned int m_uiProgram;
	unsigned int m_objProgram;
	unsigned int m_lineVBO;
	unsigned int m_objModelBuffer[2];

	//Paths
	std::string m_currentPath;
	std::string m_newPath;

	//Scale variable
	float m_scale;

	//model
	OBJModel* m_objModel;
	Line* lines;

	//Skybox
	Skybox* m_skybox;

	//Panel menus
	glm::vec3 m_backgroundColour;

};