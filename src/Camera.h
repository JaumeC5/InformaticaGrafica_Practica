//GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>

#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <vector>
#include <iostream>

using namespace glm;
using namespace std;

#pragma once


GLboolean status[1024];
GLfloat actualTime;


class Camera {
private:
	vec3 camPos;
	vec3 camFront;
	vec3 camUp;
	GLfloat deltaTime;
	GLfloat lastFrame;
	GLfloat lastX = 400;
	GLfloat lastY = 300;
	GLfloat sens;
	GLboolean firstMouse = true;
	GLfloat pitchCam = 0.0f;
	GLfloat yawCam = -90.f;
	GLfloat FOV;

public:
	Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov);

	void doMovement(GLFWwindow * window);
	void mouseMove(GLFWwindow * window, double xpos, double ypos);
	void mouseScroll(GLFWwindow * window, double xScroll, double yScroll);
	mat4 LookAt();
	GLfloat GetFOV();

	vec3 camTarget;
	vec3 camDirection;
	vec3 vecUp;
	vec3 camRight;

	
};

Camera::Camera(vec3 position, vec3 direction, GLfloat sensitivity, GLfloat fov) {
	//Constructor
	camPos = position;
	camDirection = direction;
	sens = sensitivity;
	FOV = fov;

	//Camara
	camPos = vec3(0.0f, 0.0f, 3.0f);
	camTarget = vec3(0.0f, 0.0f, 0.0f);
	camDirection = normalize(camPos - camTarget);
	vecUp = vec3(0.0f, 1.0f, 0.0f);
	camRight = normalize(cross(vecUp, camDirection));
	camUp = cross(camDirection, camRight);
	camFront = vec3(0.0f, 0.0f, -1.0f);


}

void Camera::doMovement(GLFWwindow* window) {
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
	cout << "camPos.x: " << camPos.x << endl;
	cout << "camPos.y: " << camPos.y << endl;
	cout << "camPos.z: " << camPos.z << endl;
}

void Camera::mouseMove(GLFWwindow* window, double xpos, double ypos) {

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}
	GLfloat xoffset = xpos - lastX;
	GLfloat yoffset = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	GLfloat Sensitivity = 0.03f;
	xoffset *= Sensitivity;
	yoffset *= Sensitivity;

	yawCam += xoffset;
	pitchCam += yoffset;

	if (pitchCam > 89.0f) { pitchCam = 89.0f; }
	if (pitchCam < -89.0f) { pitchCam = -89.0f; }

	vec3 front;
	front.x = cos(glm::radians(yawCam)) * cos(glm::radians(pitchCam));
	front.y = sin(glm::radians(pitchCam));
	front.z = sin(glm::radians(yawCam)) * cos(glm::radians(pitchCam));
	camFront = glm::normalize(front);
}

void Camera::mouseScroll(GLFWwindow* window, double xScroll, double yScroll) {

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

mat4 Camera::LookAt() {
	return lookAt(camPos, camPos + camFront, camUp);
}

GLfloat Camera::GetFOV() {
	return FOV;
}
