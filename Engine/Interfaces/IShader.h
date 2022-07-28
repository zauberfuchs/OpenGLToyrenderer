#pragma once

enum class ShaderType
{
	PhongShader,
	PbrShader,
	PbrTextureShader
};

class IShader
{
public:
	virtual ~IShader() = default;

	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	// Set uniforms
	virtual void SetUniform1i(const std::string& name, int value) = 0;
	virtual void SetUniform1f(const std::string& name, float value) = 0;
	virtual void SetUniform3f(const std::string& name, float v0, float v1, float v2) = 0;
	virtual void SetUniform3f(const std::string& name, glm::vec3& value) = 0;
	virtual void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) = 0;
	virtual void SetUniformMat4f(const std::string& name, glm::mat4& matrix) = 0;

	virtual unsigned int GetShaderID() = 0;

	virtual std::string GetName() = 0;
};
