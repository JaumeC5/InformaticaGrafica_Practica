//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>

#include "Camera.h"
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Object.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <math.h>
#include <SOIL.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;

GLfloat deltaTime;
GLfloat lastFrame;

float rotationAngleX = 0.0f;
float rotationAngleY = 0.0f;
vec3 objectPosition = vec3(0.0f, 0.0f, 0.0f);

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouseMove(GLFWwindow* window, double xpos, double ypos);
void mouseScroll(GLFWwindow* window, double xScroll, double yScroll);

Camera cam(vec3(0.0f, 0.0f, 3.0f), vec3(0.0f,0.0f,0.0f), 0.03f, 45.0f);

Object obj(vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), vec3(objectPosition), cube);

int main() {

	if (!glfwInit())
		exit(EXIT_FAILURE);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	GLFWwindow* window;
	window = glfwCreateWindow(WIDTH, HEIGHT, "Primera ventana", nullptr, nullptr);
	if (!window) {
		cout << "Error al crear la ventana" << endl;
		glfwTerminate();
		exit(EXIT_FAILURE);
	}
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (GLEW_OK != glewInit()) {
		std::cout << "Error al inicializar glew" << std::endl;
		glfwTerminate();
		return NULL;
	}
	
	obj.Start();

	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetScrollCallback(window, mouseScroll);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);
	glViewport(0, 0, screenWidth, screenHeight);

	glEnable(GL_DEPTH_TEST);

	Shader projectShader("./src/projectVertex.vertexshader", "./src/projectFragment.fragmentshader");

	//Model projectModel("./src/spider/spider.obj");

	float aspectRatio = WIDTH / HEIGHT;


	//While-------------------------------------------------------------------------------------------------------------------While
	while (!glfwWindowShouldClose(window)) {
		//Delta time
		actualTime = glfwGetTime();
		deltaTime = actualTime - lastFrame;
		lastFrame = actualTime;

		//Events
		glfwPollEvents();
		cam.doMovement(window, deltaTime);


		//Clear
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		projectShader.USE();
		GLint objectColorLoc = glGetUniformLocation(projectShader.Program, "objectColor");
		GLint lightColorLoc = glGetUniformLocation(projectShader.Program, "lightColor");
		glUniform3f(objectColorLoc, 1.0f, 0.5f, 0.31f);
		glUniform3f(lightColorLoc, 1.0f, 1.0f, 1.0f);

		//Camara model, view & proyection

		mat4 proj = glm::perspective(glm::radians(cam.GetFOV()), aspectRatio, 1.0f, 100.0f);
		GLint uniProj = glGetUniformLocation(projectShader.Program, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));

		GLint uniView = glGetUniformLocation(projectShader.Program, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(cam.LookAt()));
		
		GLint uniMode = glGetUniformLocation(projectShader.Program, "model");
		glUniformMatrix4fv(uniMode, 1, GL_FALSE, glm::value_ptr(obj.GetModelMatrix()));
		
		obj.Draw();
	
		glfwSwapBuffers(window);
	}

	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	//Opacidad texturas
	if (action == GLFW_PRESS) {
		status[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		status[key] = false;
	}

	if (key == GLFW_KEY_KP_4) {
		objectPosition += vec3(-1.0, 0.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_6) {
		objectPosition += vec3(1.0, 0.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_8) {
		objectPosition += vec3(0.0, 1.0f, 0.0f);
	}
	if (key == GLFW_KEY_KP_2) {
		objectPosition += vec3(0.0, -1.0f, 0.0f);
	}
	if (key == GLFW_KEY_LEFT) {
		vec3 rotateVector;
		rotateVector = vec3(1.0f, 0.0f, 0.0f);
		rotationAngleX -= 10.f;
		obj.Rotate(rotateVector, rotationAngleX);
	}
	if (key == GLFW_KEY_RIGHT) {
		vec3 rotateVector;
		rotateVector = vec3(1.0f, 0.0f, 0.0f);
		rotationAngleX += 10.f;
		obj.Rotate(rotateVector, rotationAngleX);
	}
	if (key == GLFW_KEY_UP) {
		vec3 rotateVector;
		rotateVector = vec3(0.0f, 1.0f, 0.0f);
		rotationAngleY += 10.f;
		obj.Rotate(rotateVector, rotationAngleX);
	}
	if (key == GLFW_KEY_DOWN) {
		vec3 rotateVector;
		rotateVector = vec3(0.0f, 1.0f, 0.0f);
		rotationAngleY -= 10.f;
		obj.Rotate(rotateVector, rotationAngleX);
	}

}

void mouseMove(GLFWwindow* window, double xpos, double ypos) {
	cam.mouseMove(window, xpos, ypos);
}

void mouseScroll(GLFWwindow* window, double xScroll, double yScroll) {
	cam.mouseScroll(window, xScroll, yScroll);
}



