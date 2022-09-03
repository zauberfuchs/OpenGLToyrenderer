#include "Engine/Utils/pch.h"
#include "Shader.h"

#include "Engine/Scene/World.h"


Shader::Shader(const std::string& name)
	: m_ID(0), m_Name(name)
{
	
}

Shader::Shader(const std::string& name, const std::string& vertexPath, const std::string& fragmentPath, const std::string& geometryPath)
	: m_ID(0), m_Name(name)
{
	std::string v_string = LoadShaderFile(vertexPath.c_str());
	std::string f_string = LoadShaderFile(fragmentPath.c_str());

	if(geometryPath != "")
	{
		std::string g_string = LoadShaderFile(geometryPath.c_str());
		CreateShader(v_string.c_str(), f_string.c_str(), g_string.c_str());
	}
	else
	{
		CreateShader(v_string.c_str(), f_string.c_str());
	}
	
}

void AddShaderSource(const std::string& sourcePath)
{
	
}

Shader::~Shader()
{
	glDeleteProgram(m_ID);
}

void Shader::Bind() const
{
	glUseProgram(m_ID);
}

void Shader::Unbind() const
{
	glUseProgram(0);
}

void Shader::SetUniform1i(const std::string& name, int value)
{
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetUniform1f(const std::string& name, float value)
{
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetUniform3f(const std::string& name, glm::vec3& value) {
	glUniform3fv(GetUniformLocation(name), 1, &value[0]);
}

void Shader::SetUniform3f(const std::string& name, float v0, float v1, float v2)
{
	glUniform3f(GetUniformLocation(name), v0, v1, v2);
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
	glUniform4f(GetUniformLocation(name), v0, v1, v2, v3);
}

void Shader::SetUniformMat4f(const std::string& name, glm::mat4& matrix)
{
 	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, &matrix[0][0]);
}

unsigned int Shader::GetShaderID()
{
	return m_ID;
}

int Shader::GetUniformLocation(const std::string& name)
{
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
		return m_UniformLocationCache[name];
	int location = glGetUniformLocation(m_ID, name.c_str());
	if (location == -1)
	{
		std::cout << "Warning:uniform '" << name << "' doesn't exist!" << std::endl;
	}

	m_UniformLocationCache[name] = location;
	return location;
}


void Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader, const std::string& geometryShader)
{
	unsigned int program = glCreateProgram();
	GLint isLinked = 0;
	unsigned int gs = 0;
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);
	if (geometryShader != "")
	{
		gs = CompileShader(GL_GEOMETRY_SHADER, geometryShader);
		glAttachShader(program, gs);
	}
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
	if (isLinked == GL_FALSE)
	{
		GLchar infoLog[1024];
		glGetProgramInfoLog(program, 1024, NULL, infoLog);
		std::cout << "ERROR::PROGRAM_LINKING_ERROR" << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
	}
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);
	if (geometryShader != "")
	{
		glDeleteShader(gs);
	}
	m_ID = program;
	World::Get().AddShader(this);
}


unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char* message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " sharder!" << std::endl;
		glDeleteShader(id);
		return 0;
	}
	
	return id;
}

// filepath for shaders
std::string Shader::LoadShaderFile(const char* filePath) {
	std::ifstream file;
	std::string f_code;
	std::stringstream f_stream;

	file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		file.open(filePath);
		f_stream << file.rdbuf();
		file.close();
		f_code = f_stream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "Fehler: Shader-Datei konnte nicht geladen werden." << std::endl;
		return 0;
	}
	return f_code.c_str();
}

std::string Shader::GetName()
{
	return m_Name;
}