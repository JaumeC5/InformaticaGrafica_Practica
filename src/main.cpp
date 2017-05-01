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
#include <time.h>
#include <chrono>

using namespace glm;
using namespace std;

const GLint WIDTH = 800, HEIGHT = 600;
bool textureMove;
float textOpacity = 0.9;
float rotationX = 0.0;
float rotationY = 0.0;
float rotationZ = 0.0;
float rotationCubes = 0.f;

float lastFrame = 0.0f;
float deltaTime = 0.0f;


//Movimiento camera
vec3 camPos = vec3(0.0f, 0.0f, 3.0f);
vec3 camTarget = vec3(0.0f, 0.0f, 0.0f);
vec3 camDirection = normalize(camPos - camTarget);
vec3 vecUp = vec3(0.0f, 1.0f, 0.0f);
vec3 camRight = normalize(cross(vecUp, camDirection));
vec3 camUp = cross(camDirection, camRight);
vec3 camFront = vec3(0.0f, 0.0f, -1.0f);


GLfloat yawCam = -90.f;
GLfloat pitchCam = 0.0f;
GLfloat lastX = WIDTH / 2.0f;
GLfloat lastY = HEIGHT / 2.0f;

float FOV = 45.0f;


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void doMovement(GLFWwindow* window);
void mouseMove(GLFWwindow* window, double xpos, double ypos);
void mouseScroll(GLFWwindow* window, double xScroll, double yScroll);


bool status[1024];


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
	
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouseMove);
	glfwSetScrollCallback(window, mouseScroll);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	glEnable(GL_DEPTH_TEST);

	int screenWidth, screenHeight;
	glfwGetFramebufferSize(window, &screenWidth, &screenHeight);

	glViewport(0, 0, screenWidth, screenHeight);

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(0, 0, 0, 0);

	Shader triangleShader("./src/SimpleVertexShader.vertexshader", "./src/SimpleFragmentShader.fragmentshader");
	Shader textureShader("./src/textureVertex.vertexshader", "./src/textureFragment.fragmentshader");
	Shader coordsShader("./src/coordsVertex.vertexshader", "./src/coordsFragment.fragmentshader");


	//Movimiento Camara-----------------------------------------------------------------------------------------------------------MC
	
	
	//Camara----------------------------------------------------------------------------------------------------------------------C
	float aspectRatio = WIDTH / HEIGHT;
	mat4 model;


	//Rotation & Translation-----------------------------------------------------------------------------------------------------R&T

	mat4 translationTrans;
	translationTrans = glm::translate(translationTrans, vec3(0.0f, 0.0f, 0.0f));


	mat4 rotationTransX;
	mat4 rotationTransY;
	mat4 rotationTransZ;
	//rotationTransY = glm::rotate(translationTrans, glm::radians(50.0f), vec3(0.0, 0.0, 1.0f));



	//VAO & VBO------------------------------------------------------------------------------------------------------------------V&V
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
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBufferCube), VertexBufferCube, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (GLvoid*)( 3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	//Texturas---------------------------------------------------------------------------------------------------------------Texturas
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

	//--
	GLint variableShader = glGetUniformLocation(triangleShader.Program, "angle");
	GLint moveTex = glGetUniformLocation(textureShader.Program, "opacity");



	//While-------------------------------------------------------------------------------------------------------------------While
	while (!glfwWindowShouldClose(window)) {
	
		glfwPollEvents();
		doMovement(window);

		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(0.2f, 0.2f, 0.4f, 1.0f);
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

		rotationTransX = glm::rotate(translationTrans, glm::radians(rotationX), vec3(1.0, 0.0, 0.0f));
		GLint roTransX = glGetUniformLocation(textureShader.Program, "rotationTransX");
		glUniformMatrix4fv(roTransX, 1, GL_FALSE, glm::value_ptr(rotationTransX));

		rotationTransY = glm::rotate(translationTrans, glm::radians(rotationY), vec3(0.0, 0.0, 1.0f));
		GLint roTransY = glGetUniformLocation(textureShader.Program, "rotationTransY");
		glUniformMatrix4fv(roTransY, 1, GL_FALSE, glm::value_ptr(rotationTransY));

		rotationTransZ = glm::rotate(translationTrans, glm::radians(rotationZ), vec3(0.0, 1.0, 0.0f));
		GLint roTransZ = glGetUniformLocation(textureShader.Program, "rotationTransZ");
		glUniformMatrix4fv(roTransZ, 1, GL_FALSE, glm::value_ptr(rotationTransZ));


		//Camara view & proyection
		mat4 view = glm::lookAt(camPos, camPos + camFront, camUp);
		mat4 proj = glm::perspective(glm::radians(FOV), aspectRatio, 1.0f, 100.0f);


		//Get uniform location
		GLint uniView = glGetUniformLocation(coordsShader.Program, "view");
		glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
		GLint uniProj = glGetUniformLocation(coordsShader.Program, "proj");
		glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
		GLint uniMode = glGetUniformLocation(coordsShader.Program, "model");
		glUniformMatrix4fv(uniMode, 1, GL_FALSE, glm::value_ptr(model));
		
		//Delta time
		GLfloat actualTime = glfwGetTime();
		deltaTime = actualTime - lastFrame;
		lastFrame = actualTime;
	
		
		//Opacidad texturas
		if (textureMove) {glUniform1f(moveTex, textOpacity);}
		else if (!textureMove) {glUniform1f(moveTex, textOpacity);}

		//Cubo central
		glm::mat4 trans, rot, rotX,  rotY, rotZ;
		trans = glm::translate(trans, CubesPositionBuffer[0]); 
		rotX = glm::rotate(rot, glm::radians(rotationX), glm::vec3(1.0f, 0.f, 0.0f)); 
		rotY = glm::rotate(rot, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f)); 
		rotZ = glm::rotate(rot, glm::radians(rotationZ), glm::vec3(0.0f, 0.f, 1.0f));
		rot = rotX*rotY*rotZ; 
		rotZ = glm::rotate(rot, glm::radians(rotationZ), glm::vec3(1.0f, 1.0f, 1.0f));
		model = trans * rot; 
		glUniformMatrix4fv(uniMode, 1, GL_FALSE, glm::value_ptr(model)); 
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Resto de cubos
		for (int i = 1; i < 10; i++) {
			glm::mat4 trans, rot;
			trans = glm::translate(trans, CubesPositionBuffer[i]);
			rot = glm::rotate(rot, actualTime * 0.1f * glm::radians(180.f), glm::vec3(0.3f, 1.0f, 0.0f)); 
			model = trans * rot;
			glUniformMatrix4fv(uniMode, 1, GL_FALSE, glm::value_ptr(model));
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		//triangleShader.USE();
		textureShader.USE();
		//coordsShader.USE();

		glBindVertexArray(0);
		glfwSwapBuffers(window);
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

	//Opacidad texturas
	if (key == GLFW_KEY_UP /*&& action == GLFW_PRESS*/) {
		textureMove = true;
		textOpacity += 0.1;
	}
	if (key == GLFW_KEY_DOWN /*&& action == GLFW_PRESS*/) {
		textureMove = false;
		textOpacity -= 0.1;
	}
	if (key == GLFW_KEY_1) {
		textOpacity = 1;
	}
	if (key == GLFW_KEY_2) {
		textOpacity = 0;
	}
	//Rotacion cubo central
	if (key == GLFW_KEY_I) {
		rotationX -= 10.0f;
	}
	if (key == GLFW_KEY_K) {
		rotationX += 10.0f;
	}
	if (key == GLFW_KEY_J) {
		rotationY += 10.0f;
	}
	if (key == GLFW_KEY_L) {
		rotationY -= 10.0f;
	}
	if (key == GLFW_KEY_U) {
		rotationZ += 10.0f;
	}
	if (key == GLFW_KEY_O) {
		rotationZ -= 10.0f;
	}
	if (action == GLFW_PRESS) {
		status[key] = true;
	}
	else if (action == GLFW_RELEASE) {
		status[key] = false;
	}

}


void doMovement(GLFWwindow* window) {
	float cameraSpeed = 5.0f * deltaTime;

	if (status[GLFW_KEY_W]) {
		camPos += cameraSpeed * camFront;
	}
	if (status[GLFW_KEY_S]) {
		camPos -= cameraSpeed * camFront;
	}
	if (status[GLFW_KEY_A]) {
		camPos -= normalize(cross(camFront, camUp)) * cameraSpeed;
	}
	if (status[GLFW_KEY_D]) {
		camPos += normalize(cross(camFront, camUp)) * cameraSpeed;
	}
}


bool firstMouse = true;

void mouseMove(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat sensitivity = 0.03f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yawCam += xoffset;
	pitchCam += yoffset;

	if (pitchCam > 89.0f) { pitchCam = 89.0f; }
	if (pitchCam < -89.0f) { pitchCam = -89.0f; }

	vec3 front;
	front.x = cos(glm::radians(yawCam)) * cos(glm::radians(pitchCam));
	front.y = sin(glm::radians(pitchCam));
	front.z = sin(glm::radians(yawCam)) * cos(glm::radians(pitchCam));
	camFront = glm::normalize(front);
	cout << "Cam Front X: " << camFront.x << endl;
	cout << "Cam Front Y: " << camFront.y << endl;
	cout << "Cam Front Z: " << camFront.z << endl;
}

void mouseScroll(GLFWwindow* window, double xScroll, double yScroll) {

	if (FOV >= 1.0f && FOV <= 60.0f) {	
		FOV -= yScroll; 
	}
	if (FOV <= 1.0f) { 
		FOV = 1.0f; 
	}
	if (FOV >= 60.0f) {
		FOV = 60.0f; 
	}
}
