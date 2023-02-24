#pragma once

#include <vector>
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Mesh.h"
#include "Texture.h"

class Model{
public:
	Model(const std::string& iModelName);
	~Model() { Clear(); }

	void Load(const std::string& iFilename);
	void Render();
	void Clear();

private:
	void LoadNode(aiNode* iNode, const aiScene* iScene);
	void LoadMesh(aiMesh* mesh, const aiScene* iScene);
	void LoadMaterials(const aiScene* iScene);

	std::vector<Mesh*> mMeshList;
	std::vector<Texture*> mTextureList;
	std::vector<unsigned int> mMeshToTex;

	std::string mModelName;
};