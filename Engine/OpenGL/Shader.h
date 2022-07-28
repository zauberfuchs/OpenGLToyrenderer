#pragma once
//TODO dlete glew?? move to cpp
#include <GL/glew.h>
#include "../../Engine/Interfaces/IShader.h"



class Shader : public IShader
{
public:
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath);
	~Shader() override;

	void Bind() const override;
	void Unbind() const override;

	// Set uniforms
	void SetUniform1i(const std::string& name, int value) override;
	void SetUniform1f(const std::string& name, float value) override;
	void SetUniform3f(const std::string& name, float v0, float v1, float v2) override;
	void SetUniform3f(const std::string& name, glm::vec3& value) override;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) override;
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix) override;
	
	unsigned int GetShaderID() override;

	virtual std::string GetName() override;
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
	std::string LoadShaderFile(const char* filePath);

	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_ID;

	std::string m_Name;
};

class ShaderLoader
{
public:
	ShaderLoader() = default;

	void LoadShaderFolder(const std::string& path)
	{
		std::unordered_set<std::string> filenames;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			filenames.emplace(entry.path().filename().stem().u8string());
		}
		for (const auto& filename : filenames)
		{
			Shader* s = new Shader(filename, path + filename + ".vert", path + filename + ".frag");
		}
	}
};