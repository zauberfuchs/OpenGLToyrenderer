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
	
	void AddMesh(Ref<Mesh> mesh);
	Ref<Mesh> GetMesh(int index = 0);
	std::vector<Ref<Mesh>> GetMeshes();
	

private:
	void LoadModel(std::string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Ref<Mesh> ProcessMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType typeName);
	Texture TextureFromFile(const std::string& path, TextureType type);
	Ref<Material> LoadMaterial(aiMaterial* mat);


	// model data
	std::vector<Texture> m_TexturesLoaded;
	std::vector<Ref<Mesh>> m_Meshes;
	std::string m_Directory;

	std::string m_Name;
};