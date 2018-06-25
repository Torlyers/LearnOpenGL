#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "stb_image.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"

// settings
const unsigned int SCR_POS_X = 0;
const unsigned int SCR_POS_Y = 0;//the position of the viewport
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

Camera MainCamera;
double LastX, LastY;
bool IsFirstMouse = true;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(SCR_POS_X, SCR_POS_Y, width, height);
}

void CursorPosCallback(GLFWwindow* window, double i_posX, double i_posY)
{
	if (IsFirstMouse)
	{
		LastX = i_posX;
		LastY = i_posY;
		IsFirstMouse = false;
	}

	float offsetX = i_posX - LastX;
	float offsetY = LastY - i_posY;//watch out

	LastX = i_posX;
	LastY = i_posY;

	MainCamera.ProcessMouseInput(offsetX, offsetY);
}

void MouseScrollCallback(GLFWwindow* window, double i_offsetX, double i_offsetY)
{
	MainCamera.ProcessScrollInput(i_offsetY);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		MainCamera.ProcessKeyInput(W, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		MainCamera.ProcessKeyInput(S, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		MainCamera.ProcessKeyInput(A, deltaTime);
	else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		MainCamera.ProcessKeyInput(D, deltaTime);
}

int GenerateTexture(const char* image_path)//可以用类封装一下
{
	unsigned int texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);	

	//设置纹理属性
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);//x轴，重复平铺
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);//y轴，重复平铺
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);//缩小时使用临近过滤
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);//放大时使用线性过滤

	//load image
	stbi_set_flip_vertically_on_load(true);//coordinate of image and OpenGL is different
	int width, height, nrChannels;
	unsigned char *data = stbi_load(image_path, &width, &height, &nrChannels, 0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
	glGenerateMipmap(GL_TEXTURE_2D);

	stbi_image_free(data);//free image after generating textures

	return texture;
}


int main()
{
	glfwInit();
	//Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core Mode
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
	
	//set frame size callback function
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	//set input callbacks
	glfwSetCursorPosCallback(window, CursorPosCallback);
	glfwSetScrollCallback(window, MouseScrollCallback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	//Init GLAD to enable openGL functions' pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	//enable z-buffer
	glEnable(GL_DEPTH_TEST);

	//render size, set it to window size
	glViewport(SCR_POS_X, SCR_POS_Y, SCR_WIDTH, SCR_HEIGHT);
	
	Shader shader("Shaders/test.vert", "Shaders/test.frag");
	Shader LightShader("Shaders/light.vert", "Shaders/light.frag");

	Model gameobj("Assets/nanosuit/nanosuit.obj");

	float CubeVertices[] =
	{
		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f, -0.5f,  
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f, -0.5f,  0.5f, 
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f, -0.5f,  0.5f, 

		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f, -0.5f, 
		-0.5f, -0.5f,  0.5f, 
		-0.5f,  0.5f,  0.5f, 

		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  

		-0.5f, -0.5f, -0.5f, 
		0.5f, -0.5f, -0.5f,  
		0.5f, -0.5f,  0.5f,  
		0.5f, -0.5f,  0.5f,  
		-0.5f, -0.5f,  0.5f, 
		-0.5f, -0.5f, -0.5f, 

		-0.5f,  0.5f, -0.5f, 
		0.5f,  0.5f, -0.5f,  
		0.5f,  0.5f,  0.5f,  
		0.5f,  0.5f,  0.5f,  
		-0.5f,  0.5f,  0.5f, 
		-0.5f,  0.5f, -0.5f
	};
	
	
	unsigned LightVAO;
	glGenVertexArrays(1, &LightVAO);
	
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);
	
	glBindVertexArray(LightVAO);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// texture coord attribute
	// 	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	// 	glEnableVertexAttribArray(2);

	//unsigned int texture1 = GenerateTexture("Assets/1.jpg");

	// 	shader.use();
	// 	shader.setInt("texture_diffuse1", 0);

	vec3 LightPos(5.0f, -2.0f, 1.0f);

	//render loop
	while (!glfwWindowShouldClose(window))
	{
		//calculate dt
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		//input
		processInput(window);

		//render background
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);//deep green
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//use deep green to clear the screen

// 		glActiveTexture(GL_TEXTURE0);
// 		glBindTexture(GL_TEXTURE_2D, texture1);
		//transform
		double timeValue = glfwGetTime();

		shader.use();
		
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(5.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shader.setMat4("model", model);
		
		glm::mat4 view = MainCamera.GetViewMatrix();
		shader.setMat4("view", view);
		glm::mat4 projection = glm::perspective(MainCamera.GetZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);		
		shader.setMat4("projection", projection);

		//render
		gameobj.Draw(shader);


		LightShader.use();
		// 		LightShader.setVec3("objectColor", 1.0f, 0.5f, 0.31f);
		// 		LightShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

		glm::mat4 LightModel;
		LightModel = glm::translate(LightModel, LightPos);
		LightModel = glm::scale(LightModel, glm::vec3(0.2f, 0.2f, 0.2f));
		LightShader.setMat4("model", LightModel);
		LightShader.setMat4("view", view);
		LightShader.setMat4("projection", projection);
		glBindVertexArray(LightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//swap buffers
		glfwSwapBuffers(window);//when one frame is rendered in the back end, swap it to front end	
		//check events
		glfwPollEvents();			
	}

	glDeleteVertexArrays(1, &LightVAO);
	glDeleteBuffers(1, &VBO);

	//release resources;
	glfwTerminate();

	return 0;
}