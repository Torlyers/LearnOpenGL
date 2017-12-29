#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include"ShaderManager.h"

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		std::cout << "Press A" << std::endl;
}


int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Create Window
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
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
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

	//set frame size callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//Init shader manager
	ShaderManager shader_manager("Shaders/test.vs", "Shaders/test.frs");

	//define a triangle
	float vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};
	
	// bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);		
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	//glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	//glBindVertexArray(0);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//input
		processInput(window);

		//render
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//deep green
		glClear(GL_COLOR_BUFFER_BIT);//use deep green to clear the screen

		shader_manager.use();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//check events
		glfwPollEvents();

		//swap buffers
		glfwSwapBuffers(window);//when one frame is rendered in the back end, swap it to front end		
	}

	//release resources;
	glfwTerminate();

	return 0;
}