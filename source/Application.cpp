#include "Application.h"
#include "Utilities.h"
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>


bool Application::create(const char* a_name, int a_width, int a_height, int a_argc, char* a_argv[])
{
	// initialise glfw systems
	if (glfwInit() != GL_TRUE)
	{
		std::cout << "Unable to initialize GLFW" << std::endl;
		return false;
	}
	
	// create a windowed mode window and its OpenGL context
	glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
	GLFWmonitor* m_pMonitor = (false) ? glfwGetPrimaryMonitor() : NULL;
	m_window = glfwCreateWindow(a_width, a_height, a_name, m_pMonitor, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Unable to create a GLFW Window" << std::endl;
		glfwTerminate();
		return false;
	}

	if (m_pMonitor != nullptr)
	{
		glfwSetWindowPos(m_window, 100, 100);
	}
	glfwShowWindow(m_window);

	int major = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MAJOR);
	int minor = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_VERSION_MINOR);
	int revision = glfwGetWindowAttrib(m_window, GLFW_CONTEXT_REVISION);
	std::cout << "OpenGL Version " << major << "." << minor << "." << revision << std::endl;

	// make the window's context current
	glfwMakeContextCurrent(m_window);


	// initialise glew systems to wrangle GL extensions
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Unable to initialize GLEW" << std::endl;
		glfwTerminate();
		return false;
	}

	bool result = onCreate(a_argc,a_argv);
	if (result == false)
	{
		glfwTerminate();
	}
	return result;
}

void Application::run()
{
	Utility::resetTimer();
	m_running = true;
	do
	{
		float deltaTime = Utility::tickTimer();

		onUpdate( deltaTime );

		onDraw();

		glfwSwapBuffers(m_window);
		glfwPollEvents();

	} while (m_running == true && glfwWindowShouldClose(m_window) == 0);

	onDestroy();

	glfwTerminate();
}