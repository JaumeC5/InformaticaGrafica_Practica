#pragma once

#include "Mesh.h"
#include <SOIL.h>
#include <assimp\scene.h>
#include <assimp\postprocess.h>

class Model {

private:
	vector <Mesh> meshes;
	string directory;

	void loadModel(string path);
	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);

public:
	Model();
	Model(GLchar* path);
	void Draw(Shader shader, GLint drawMode);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);

};