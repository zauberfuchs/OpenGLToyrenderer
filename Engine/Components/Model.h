#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include "Mesh.h"
#include "Engine/OpenGL/Texture.h"
#include "Engine/Components/Material.h"

class Model
{
public:
	// stores all the textures loaded so far, 
	// optimization to make sure textures aren't loaded more than once.
	Model() = default;
	Model(const std::string& name);
	Model(const char* path);

	~Model();
	
	void AddMesh(Mesh* mesh);
	Mesh* GetMesh(const int& index);
	Mesh* GetMesh(const std::string& name);
	std::unordered_map<std::string, Mesh*> GetMeshes();

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	Texture TextureFromFile(const std::string& path, TextureType type);
	Material* LoadMaterial(aiMaterial* mat);


	// model data
	std::vector<Texture> m_TexturesLoaded;
	std::unordered_map<std::string, Mesh*> m_Meshes;
	std::string m_Directory;

	std::string m_Name;
};