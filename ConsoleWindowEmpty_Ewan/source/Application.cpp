#include "Application.h"
#include "ApplicationEvent.h"
#include "Observer.h"
#include "ShaderUtil.h"
#include "Utilities.h"

//Include the OpenGL Header
#include <glad/glad.h>
//Include GLFW header
#include <GLFW/glfw3.h>
//Include imgui header
#include <imgui.h>
#include <imgui_impl_opengl3.h>
#include <imgui_impl_glfw.h>

//include iostream for console logging
#include <iostream>

bool Application::Create(const char* a_applicationName, unsigned int a_windowWidth, unsigned int a_windowHeight, bool a_fullscreen)
{
	//Initialise GLFW
	if (!glfwInit()) { return false; }

	m_windowWidth = a_windowWidth;
	m_windowHeight = a_windowHeight;
	//Create a windowed mode window and it's OpenGL context
	m_window = glfwCreateWindow(m_windowWidth, m_windowHeight, a_applicationName,
		(a_fullscreen ? glfwGetPrimaryMonitor() : nullptr), nullptr);
	if (!m_window)
	{
		glfwTerminate();
		return false;
	}

	//Make the window's context current
	glfwMakeContextCurrent(m_window);

	//Initialise GLAD - Load in GL Extensions
	if (!gladLoadGL())
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
		return false;
	}
	//Get the supported OpenGl version
	int major = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_REVISION);

	std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;

	//Set up glfw window resize callback functiom
	glfwSetWindowSizeCallback(m_window, [](GLFWwindow*, int w, int h)
	{
		//Call the global dispacther to handle this function
		Dispatcher* dp = Dispatcher::GetInstance();
		if (dp != nullptr)
		{
			dp->publish(new WindowResizeEvent(w, h), true);
		}
	});

	//Create Dispatcher
	Dispatcher::CreateInstance();

	//Set up IMGUI
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	//set ImGui style
	ImGui::StyleColorsDark();
	const char* glsl_version = "#version 150";
	ImGui_ImplGlfw_InitForOpenGL(m_window, true);
	ImGui_ImplOpenGL3_Init(glsl_version);


	bool result = onCreate();
	if (result == false)
	{
		glfwDestroyWindow(m_window);
		glfwTerminate();
	}
	return result;
}

void Application::Run(const char* a_name, unsigned int a_width, unsigned int a_height, bool a_fullscreen)
{
	if (Create(a_name, a_width, a_height, a_fullscreen))
	{
		Utility::resetTimer();
		m_running = true;
		do
		{
			float deltaTime = Utility::tickTimer();
			//start the Imgui frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			showFrameData(true);

			Update(deltaTime);

			Draw();

			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			glfwSwapBuffers(m_window);
			glfwPollEvents();
		} while (m_running == true && glfwWindowShouldClose(m_window) == 0);

		Destroy();
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}
	Dispatcher::DestroyInstance();
	ShaderUtil::DestroyInstance();
	//Cleanup
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void  Application::showFrameData(bool a_bShowFrameData) 
{
	const float DISTANCE = 10.f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	//Positions the window in the top left corner of the screen no matter the size
	ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE,(corner & 2) ? io.DisplaySize.y - DISTANCE :DISTANCE);
	ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.f : 0.f, (corner & 2) ? 1.f : 0.f);

	ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	ImGui::SetNextWindowBgAlpha(0.3f);
	
	//Opens the window and sets it up so it doesn't move and can't be resized
	if (ImGui::Begin("Frame Data", &a_bShowFrameData, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | 
		ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Separator();
		ImGui::Text("Application Average: %.3f ms/frame (%.1f FPS)", 1000.f/io.Framerate, io.Framerate);
		if (ImGui::IsMousePosValid())
		{
			ImGui::Text("Mouse Position: (%.1f, %.1f)", io.MousePos.x, io.MousePos.y);
		}
		else
		{
			ImGui::Text("Mouse Position: Invalid");
		}
		ImGui::End();
	}

}