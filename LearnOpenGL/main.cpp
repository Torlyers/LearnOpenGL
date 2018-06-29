#include <iostream>

#include<glad/glad.h>
#include<GLFW/glfw3.h>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Light.h"
#include "File.h"
#include "SkyBox.h"

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

int main()
{
	glfwInit();
	//Version 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//Core Mode
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//sample mode
	glfwWindowHint(GLFW_SAMPLES, 4);

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
	glDepthFunc(GL_LESS);
	
	//enable stencil test
	glEnable(GL_STENCIL_TEST);
	glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	//enable face calling
	glEnable(GL_CULL_FACE);

	//enable multi-sample
	glEnable(GL_MULTISAMPLE);
	
	//render size, set it to window size
	glViewport(SCR_POS_X, SCR_POS_Y, SCR_WIDTH, SCR_HEIGHT);
	
	//Shaders
	Shader ModelShader("Shaders/test.vert", "Shaders/test.frag");
	Shader SingleColorShader("Shaders/test.vert", "Shaders/SingleColor.frag");
	Shader SkyBoxShader("Shaders/Skybox.vert", "Shaders/Skybox.frag");
	Shader ReflectShader("Shaders/Reflect.vert", "Shaders/Reflect.frag");

	//Objects
	Model gameobj("Assets/nanosuit/nanosuit.obj");
	Model ReflectObj = gameobj;
	Model RefractObj = gameobj;
	
	vec3 GameObjPos(5.0f, 2.0f, 0.0f);
	vec3 ReflectObjPos(-5.0f, 2.0f, 0.0f);
	vec3 RefractObjPos(15.0f, 2.0f, 0.0f);

	SkyBox skybox;

	
	vec3 LightPos(5.0f, -1.5f, 1.0f);
	vec3 LightColor(1.0f, 1.0f, 1.0f);
	vec3 ParaColor(1.0f, 1.0f, 0.8f);
	vec3 LightDir(-0.2f, -1.0f, -0.3);
	float LightRange = 20.0f;
	PointLight pol(LightColor, LightPos, LightRange);
	ParallelLight pal(ParaColor, LightPos, LightDir);

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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);//use deep green to clear the screen

		//transform
		double timeValue = glfwGetTime();		
		
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(5.0f, 2.0f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		glm::mat4 view = MainCamera.GetViewMatrix();
		glm::mat4 projection = glm::perspective(MainCamera.GetZoom(), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		vec3 LightPos1 = LightPos + vec3(0.0f, 0.0f, sin(timeValue));		

		//render		

		//model
		ModelShader.use();
		ModelShader.setMat4("model", model);
		ModelShader.setMat4("view", view);
		ModelShader.setMat4("projection", projection);
		ModelShader.setVec3("viewPos", MainCamera.GetPosition());

		ModelShader.setVec3("material.ambient", 1.0f, 1.0f, 0.8f);
		//shader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		//shader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		ModelShader.setFloat("material.shininess", 32.0f);
		
		ModelShader.setVec3("pointLight.position", LightPos1);
		ModelShader.setVec3("pointLight.ambient", pol.GetColor() * 0.8f);
		ModelShader.setVec3("pointLight.diffuse", pol.GetColor() * 0.8f);
		ModelShader.setVec3("pointLight.specular", pol.GetColor() * 0.8f);

		float c, l, q;
		pol.CalAttParameters(c, l, q);
		ModelShader.setFloat("pointLight.constant", c);
		ModelShader.setFloat("pointLight.linear", l);
		ModelShader.setFloat("pointLight.quadratic", q);

		ModelShader.setVec3("paraLight.direction", LightDir);
		ModelShader.setVec3("paraLight.ambient", pal.GetColor() * 0.2f);
		ModelShader.setVec3("paraLight.diffuse", pal.GetColor() * 0.2f);
		ModelShader.setVec3("paraLight.specular", pal.GetColor() * 0.2f);

// 		glStencilFunc(GL_ALWAYS, 1, 0xFF);
// 		glStencilMask(0xFF);//将模版缓冲模型覆盖的部分都写为1
		ModelShader.use();
		gameobj.Draw(ModelShader);

		//Reflect model
		model = translate(model, -GameObjPos);
		model = translate(model, ReflectObjPos);
		ModelShader.setMat4("model", model);
		ReflectObj.Draw(ModelShader);

		model = translate(model, -ReflectObjPos);
		model = translate(model, RefractObjPos);
		ModelShader.setMat4("model", model);
		RefractObj.Draw(ModelShader);
		
		//skybox
		glDepthFunc(GL_LEQUAL);
		SkyBoxShader.use();
		SkyBoxShader.setInt("skybox", 0);
		glm::mat4 view1 = glm::mat4(glm::mat3(MainCamera.GetViewMatrix()));
		SkyBoxShader.setMat4("view", view1);
		SkyBoxShader.setMat4("projection", projection);
		skybox.Draw(SkyBoxShader);
		glDepthFunc(GL_LESS);


// 		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);//只绘制模板值不为1的部分
// 		glStencilMask(0x00);
// 		glDisable(GL_DEPTH_TEST);
// 		SingleColorShader.use();
// 		model = glm::scale(model, glm::vec3(1.01f, 1.01f, 1.01f));
// 		SingleColorShader.setMat4("model", model);
// 		SingleColorShader.setMat4("view", view);
// 		SingleColorShader.setMat4("projection", projection);
// 		gameobj.Draw(SingleColorShader);
// 		glStencilMask(0xFF);
// 		glEnable(GL_DEPTH_TEST); //完成之后重新启用深度缓冲		

		//swap buffers
		glfwSwapBuffers(window);//when one frame is rendered in the back end, swap it to front end	
		//check events
		glfwPollEvents();			
	}

	//release resources;
	glfwTerminate();

	return 0;
}