#pragma once
#include <GL\glew.h>
#include<glm.hpp>
#include <gtc\matrix_transform.hpp>

#include "Shader.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

using namespace glm;


enum FigureType {
	cube = 0,
};

class Object {

public:
	glm::mat4 model;

	vec3 objPosition;
	vec3 objScale;
	vec3 objRotation;

	float rotX = 0.0f;
	float rotY = 0.0f;

	Object(vec3 scale, vec3 rotation, vec3 position, FigureType typef);
	~Object();

	void Start();
	void Draw();
	void Move(vec3 translation);
	void Rotate(vec3 rota, float rotationAngle);
	void Scale(vec3 scal);
	void Delete();
	mat4 GetModelMatrix();
	//vec3 GetPosition();

private:
	GLuint VBO, VAO, EBO;
	vec3 position;
	vec3 scale;
	vec3 rotation;

};

