#pragma once

#ifndef SHADER_H
#define SHADER_H

////GLEW
#define GLEW_STATIC
#include <GL\glew.h>
//GLFW
#include <GLFW\glfw3.h>


#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

using namespace std;



class Shader {
	
public:
	GLuint Program;
	Shader(const GLchar* vertexPath, const GLchar* fragmentPath);
	void USE() {
		glUseProgram(Program);
	}
};

#endif