#include "Model.h"

Model::Model(const std::string& iModelName)
	:mModelName(iModelName)
{

}

void Model::Load(const std::string& iFilename)
{
	Assimp::Importer wImporter;

	const aiScene* wScene = wImporter.ReadFile(iFilename,
		aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_GenSmoothNormals |
		aiProcess_JoinIdenticalVertices);

	if (!wScene)
	{
		printf("Model %s Failed to Load : %s", iFilename, wImporter.GetErrorString());
		return;
	}

	LoadNode(wScene->mRootNode, wScene);
	LoadMaterials(wScene);
}

void Model::Render()
{
	for (size_t i = 0; i < mMeshList.size(); i++)
	{
		unsigned int wMaterialIndex = mMeshToTex[i];
		if(wMaterialIndex < mTextureList.size() && mTextureList[wMaterialIndex])
		{
			mTextureList[wMaterialIndex]->Use(0);
		}
		mMeshList[i]->Render();
	}
}

void Model::Clear()
{
	for (size_t i = 0; i < mMeshList.size(); i++)
	{
		if (mMeshList[i])
		{
			delete mMeshList[i];
			mMeshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < mTextureList.size(); i++)
	{
		if (mTextureList[i])
		{
			delete mTextureList[i];
			mTextureList[i] = nullptr;
		}
	}
}

void Model::LoadNode(aiNode* iNode, const aiScene* iScene)
{
	for (size_t i = 0; i < iNode->mNumMeshes; i++)
	{
		LoadMesh(iScene->mMeshes[iNode->mMeshes[i]], iScene);
	}

	for (size_t i = 0; i < iNode->mNumChildren; i++)
	{
		LoadNode(iNode->mChildren[i], iScene);
	}
}

void Model::LoadMesh(aiMesh* iMesh, const aiScene* iScene)
{
	std::vector<GLfloat> wVertices;
	wVertices.reserve(iMesh->mNumVertices * 8);
	std::vector<unsigned int> wIndices;

	for (size_t i = 0; i < iMesh->mNumVertices; i++)
	{
		// Add Position
		wVertices.push_back(iMesh->mVertices[i].x);
		wVertices.push_back(iMesh->mVertices[i].y);
		wVertices.push_back(iMesh->mVertices[i].z);

		// Add UV coords
		if (iMesh->mTextureCoords[0]) {
			wVertices.push_back(iMesh->mTextureCoords[0][i].x);
			wVertices.push_back(iMesh->mTextureCoords[0][i].y);
		}
		else
		{
			wVertices.push_back(0.0f);
			wVertices.push_back(0.0f);
		}

		// Add Normals
		wVertices.push_back(-iMesh->mNormals[i].x);
		wVertices.push_back(-iMesh->mNormals[i].y);
		wVertices.push_back(-iMesh->mNormals[i].z);
	}

	// Add Indices
	for (size_t i = 0; i < iMesh->mNumFaces; i++)
	{
		aiFace wFace = iMesh->mFaces[i];
		for (size_t j = 0; j < wFace.mNumIndices; j++)
		{
			wIndices.push_back(wFace.mIndices[j]);
		}
	}

	Mesh* wMesh = new Mesh();
	wMesh->Create(wVertices.data(), wIndices.data(), wVertices.size(), wIndices.size());

	mMeshList.push_back(wMesh);
	mMeshToTex.push_back(iMesh->mMaterialIndex);

}

void Model::LoadMaterials(const aiScene* iScene)
{
	mTextureList.resize(iScene->mNumMaterials);

	for (size_t i = 0; i < iScene->mNumMaterials; i++)
	{
		aiMaterial* wMaterial = iScene->mMaterials[i];
		mTextureList[i] = nullptr;
		if (wMaterial->GetTextureCount(aiTextureType_DIFFUSE) > 0)
		{
			aiString wPath;
			if (wMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &wPath) == AI_SUCCESS) {
				int idx = std::string(wPath.data).rfind("\\");
				std::string wFilename = std::string(wPath.data).substr(idx + 1);

				std::string wTexPath = std::string("resources/Textures/") + mModelName + "/" + wFilename;

				mTextureList[i] = new Texture(wTexPath.data(), false);

				if (!mTextureList[i]->Load())
				{
					printf("Failed to texture at : %s", wTexPath);
					delete mTextureList[i];
					mTextureList[i] = nullptr;
				}
			}
		}
		if (!mTextureList[i])
		{
			mTextureList[i] = new Texture("resources/Textures/plain.png", true);
			mTextureList[i]->Load();
		}
	}
}

