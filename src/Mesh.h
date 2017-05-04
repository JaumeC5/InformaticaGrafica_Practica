#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "Shader.h"
#include <gl\glew.h>
#include <gtc\matrix_transform.hpp>
#include <glm.hpp>
#include <assimp\types.h>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	GLuint id;
	string type;
};

class Mesh {
private:
	GLuint VAO, VBO, EBO;
	void setupMesh();

public:
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vert, vector<GLuint> ind, vector <Texture> text);
	
	void Draw(Shader MeshShader, GLint DrawMode);

};