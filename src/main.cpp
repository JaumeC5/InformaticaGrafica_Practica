//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>

#include "Shader.h"
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
bool WIDEFRAME = false;
bool textureMove;
float textOpacity = 0.8;
float rotationX = 0.0;
float rotationY = 0.0;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);


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

	glEnable(GL_DEPTH_TEST);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glViewport(0, 0, screenWidth, screenHeight);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	


	Shader triangleShader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");

	Shader textureShader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");

	Shader coordsShader("./src/coordsVertex.vertexshader", "./src/coordsFragment.fragmentshader");


	//Camara -----------------
	float aspectRatio = 16.0f / 9.0f;
	float FOV = 60.0f;
	mat4 proj = glm::perspective(glm::radians(FOV), aspectRatio, 0.1f, 100.0f);
	//mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -2.f));
	glm::mat4 view = glm::lookAt(
		glm::vec3(2.0f, 2.0f, 1.5f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.5f)
	);
	//mat4 model;
	

	mat4 scaleTrans;
	scaleTrans = glm::scale(scaleTrans, vec3(0.5f, 0.5f, 0.0f));

	mat4 translationTrans;
	//translationTrans = glm::translate(translationTrans, vec3(0.0f, 0.0f, 0.0f));
	translationTrans = glm::translate(translationTrans, vec3(0.0f, 0.0f, 0.0f));//


	mat4 rotationTransX;
	mat4 rotationTransY;
	//rotationTransY = glm::rotate(translationTrans, glm::radians(50.0f), vec3(0.0, 0.0, 1.0f));

	mat4 translatePlane;
	translatePlane = glm::translate(translatePlane, vec3(0.0f, 0.0f, 0.0f));

	mat4 rotatePlane;
	rotatePlane = glm::rotate(translationTrans, glm::radians(50.0f), vec3(0.0, 0.0, 1.0f));



	GLuint VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	
	GLuint VBO;
	glGenBuffers(1, &VBO);


	GLfloat VertexBufferCube[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f , -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f , -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f ,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f ,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	vec3 CubesPositionBuffer[] = {
		vec3(0.0f ,  0.0f,  0.0f),
		vec3(2.0f ,  5.0f, -15.0f),
		vec3(-1.5f, -2.2f, -2.5f),
		vec3(-3.8f, -2.0f, -12.3f),
		vec3(2.4f , -0.4f, -3.5f),
		vec3(-1.7f,  3.0f, -7.5f),
		vec3(1.3f , -2.0f, -2.5f),
		vec3(1.5f ,  2.0f, -2.5f),
		vec3(1.5f ,  0.2f, -1.5f),
		vec3(-1.3f,  1.0f, -1.5f)
	};

	//---


	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);


	glBindTexture(GL_TEXTURE_2D, 0);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)( 3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);



	glfwSetKeyCallback(window, key_callback);

	GLuint texture1;
	glGenTextures(1, &texture1);
	glBindTexture(GL_TEXTURE_2D, texture1);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int textureWidth, textureHeight;
	unsigned char* image;

	image = SOIL_load_image("./src/textureCena.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	GLuint texture2;
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	image = SOIL_load_image("./src/texture.png", &textureWidth, &textureHeight, 0, SOIL_LOAD_RGB);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);

	//glBindVertexArray(0);

	GLint variableShader = glGetUniformLocation(triangleShader.Program, "angle");
	GLint moveTex = glGetUniformLocation(textureShader.Program, "opacity");



	while (!glfwWindowShouldClose(window)) {


		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glBindVertexArray(VAO);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);



		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture1);
		glUniform1i(glGetUniformLocation(textureShader.Program, "Texture1"), 0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, texture2);
		glUniform1i(glGetUniformLocation(textureShader.Program, "Texture2"), 1);


		rotationTransX = glm::rotate(translationTrans, glm::radians(rotationX), vec3(0.0, 1.0, 0.0f));
		GLint roTransX = glGetUniformLocation(textureShader.Program, "rotationTransX");
		glUniformMatrix4fv(roTransX, 1, GL_FALSE, glm::value_ptr(rotationTransX));

		rotationTransY = glm::rotate(translationTrans, glm::radians(rotationY), vec3(0.0, 0.0, 1.0f));
		GLint roTransY = glGetUniformLocation(textureShader.Program, "rotationTransY");
		glUniformMatrix4fv(roTransY, 1, GL_FALSE, glm::value_ptr(rotationTransY));


		GLint scTrans = glGetUniformLocation(textureShader.Program, "scaleTrans");
		glUniformMatrix4fv(scTrans, 1, GL_FALSE, glm::value_ptr(scaleTrans));

		GLint trTrans = glGetUniformLocation(textureShader.Program, "translationTrans");
		glUniformMatrix4fv(trTrans, 1, GL_FALSE, glm::value_ptr(translationTrans));

		//Plano
		GLint roPlane = glGetUniformLocation(textureShader.Program, "rotatePlane");
		glUniformMatrix4fv(scTrans, 1, GL_FALSE, glm::value_ptr(rotatePlane));

		GLint trPlane = glGetUniformLocation(textureShader.Program, "translatePlane");
		glUniformMatrix4fv(trTrans, 1, GL_FALSE, glm::value_ptr(translatePlane));
		//


		//Camara--
		GLint uniView = glGetUniformLocation(coordsShader.Program, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));

		GLint uniProj = glGetUniformLocation(coordsShader.Program, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
		//--



		textureShader.USE();
		

		if (textureMove) {
			glUniform1f(moveTex, textOpacity);
		}

		else if (!textureMove) {
			glUniform1f(moveTex, textOpacity);
		}

		

		glBindVertexArray(0);
		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	
	glDeleteTextures(1, &texture1);
	glDeleteTextures(1, &texture2);

	exit(EXIT_SUCCESS);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_P && action == GLFW_PRESS) {
		if (WIDEFRAME) {
			WIDEFRAME = false;
		}
		else { WIDEFRAME = true; }
	}

	if (key == GLFW_KEY_UP && action == GLFW_PRESS) {
		textureMove = true;
		textOpacity += 0.1;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS) {
		textureMove = false;
		textOpacity -= 0.1;
	}
	if (key == GLFW_KEY_LEFT) {
		rotationY += 10.0f;
	}
	if (key == GLFW_KEY_RIGHT) {
		rotationY -= 10.0f;
	}
	if (key == GLFW_KEY_1) {
		textOpacity = 1;
	}
	if (key == GLFW_KEY_2) {
		textOpacity = 0;
	}
	if (key == GLFW_KEY_W) {
		rotationX -= 10.0f;
	}
	if (key == GLFW_KEY_S) {
		rotationX += 10.0f;
	}
	if (key == GLFW_KEY_A) {
		rotationY += 10.0f;
	}
	if (key == GLFW_KEY_D) {
		rotationY -= 10.0f;
	}

}


