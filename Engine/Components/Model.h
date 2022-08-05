#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>

#include "Mesh.h"
#include "../../Engine/OpenGL/Texture.h"
#include "../../Engine/Scene/Camera.h"
#include "../../Engine/Interfaces/ISceneObject.h"
#include "../../Engine/Interfaces/IModel.h"

class Model : public IModel
{
public:
	// stores all the textures loaded so far, 
	// optimization to make sure textures aren't loaded more than once.
	Model(const std::string& name);
	Model(const char* path);

	~Model() override;
	
	void AddMesh(IMesh* mesh) override;
	IMesh* GetMesh(const int& index) override;
	IMesh* GetMesh(const std::string& name) override;
	std::unordered_map<std::string, IMesh*> GetMeshes() override;

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	IMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	Texture TextureFromFile(const std::string& path, TextureType type);
	Material* LoadMaterial(aiMaterial* mat);


	// model data
	std::vector<Texture> m_TexturesLoaded;
	std::unordered_map<std::string, IMesh*> m_Meshes;
	std::string m_Directory;

	std::string m_Name;
};