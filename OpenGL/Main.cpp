#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	//Init GLAD to enable openGL functions pointer
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//render size, set it to window size
	glViewport(0, 0, 800, 600);

	//set frame size callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render

		//check events
		glfwPollEvents();

		//swap buffers
		glfwSwapBuffers(window);//when one frame is rendered in the back end, swap it to front end		
	}

	//release resources;
	glfwTerminate();

	return 0;
}

