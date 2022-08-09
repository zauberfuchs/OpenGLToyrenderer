#pragma once
//TODO dlete glew?? move to cpp
#include <GL/glew.h>
#include "../../Engine/Interfaces/IShader.h"



class Shader : public IShader
{
public:
	Shader(const std::string& name);
	Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath = "");
	~Shader() override;

	void Bind() const override;
	void Unbind() const override;

	void AddShaderSource(const std::string& sourcePath);

	// Set uniforms
	void SetUniform1i(const std::string& name, int value) override;
	void SetUniform1f(const std::string& name, float value) override;
	void SetUniform3f(const std::string& name, float v0, float v1, float v2) override;
	void SetUniform3f(const std::string& name, glm::vec3& value) override;
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) override;
	void SetUniformMat4f(const std::string& name, glm::mat4& matrix) override;
	
	unsigned int GetShaderID() override;
	void AddUniform(std::string name, GLenum type);

	std::string GetName() override;
	void CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryPath = "");
private:
	unsigned int CompileShader(unsigned int type, const std::string& source);
	int GetUniformLocation(const std::string& name);
	std::string LoadShaderFile(const char* filePath);

	std::unordered_map<std::string, int> m_UniformLocationCache;
	unsigned int m_ID;

	std::string m_Name;
};

class ShaderLoader
{
public:

	static void LoadShaderFolder(const std::string& path)
	{
		std::unordered_map<std::string, uint16_t> filenames;
		for (const auto& entry : std::filesystem::directory_iterator(path))
		{
			if(entry.path().filename().extension() == ".geom")
			{
				filenames.insert_or_assign(entry.path().filename().stem().u8string(), 0x001);
			}
			else
			{
				filenames.insert({ entry.path().filename().stem().u8string(), NULL });
			}
		}
		for (const auto& filename : filenames)
		{
			if(filename.second == 0x001)
			{
				Shader* s = new Shader(filename.first, path + filename.first + ".vert", path + filename.first + ".frag", path + filename.first + ".geom");
			}
			else
			{
				Shader* s = new Shader(filename.first, path + filename.first + ".vert", path + filename.first + ".frag");
			}
		}
	}
};