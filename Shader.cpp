#include "Shader.h"

using namespace std;

void Shader::CheckErrors(unsigned int& ID, string type)
{
	if (type != "PROGRAM")
	{
		glGetShaderiv(ID, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(ID, 1024, NULL, infoLog);
			cout << type << " SHADER COMPILATION FAILURE: " << infoLog << endl;
		}
	}
	else
	{
		glGetProgramiv(ID, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(ID, 1024, NULL, infoLog);
			cout << "SHADER PROGRAM LINKING FAILURE: " << infoLog << endl;
		}
	}
}

Shader::Shader(const char* VertexPath, const char* FragmentPath)
{
	vertexShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fragmentShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	try
	{
		vertexShaderFile.open(VertexPath);
		fragmentShaderFile.open(FragmentPath);
		vertexCodeStream << vertexShaderFile.rdbuf();
		fragmentCodeStream << fragmentShaderFile.rdbuf();
		vertexShaderFile.close();
		fragmentShaderFile.close();
		vertexCodeS = vertexCodeStream.str();
		fragmentCodeS = fragmentCodeStream.str();
	}
	catch (...)
	{
		cout << "SHADER FILE READING FAILURE" << endl;
	}
	vertexCode = vertexCodeS.c_str();
	fragmentCode = fragmentCodeS.c_str();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &vertexCode, NULL);
	glCompileShader(vertexShaderID);
	CheckErrors(vertexShaderID, "VERTEX");

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &fragmentCode, NULL);
	glCompileShader(fragmentShaderID);
	CheckErrors(fragmentShaderID, "FRAGMENT");

	ShaderProgramID = glCreateProgram();
	glAttachShader(ShaderProgramID, vertexShaderID);
	glAttachShader(ShaderProgramID, fragmentShaderID);
	glLinkProgram(ShaderProgramID);
	CheckErrors(ShaderProgramID, "PROGRAM");

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);
}
unsigned int Shader::GetProgramID()
{
	return ShaderProgramID;
}
void Shader::use()
{
	glUseProgram(ShaderProgramID);
}
void Shader::setInt(const string& name, int value) const
{
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void Shader::setFloat(const string& name, float value) const
{
	glUniform1f(glGetUniformLocation(ShaderProgramID, name.c_str()), value);
}
void Shader::setBool(const string& name, bool value) const
{
	glUniform1i(glGetUniformLocation(ShaderProgramID, name.c_str()), (int)value);
}
// ------------------------------------------------------------------------
void Shader::setVec2(const string& name, const glm::vec2& value) const
{
	glUniform2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec2(const string& name, float x, float y) const
{
	glUniform2f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y);
}
// ------------------------------------------------------------------------
void Shader::setVec3(const string& name, const glm::vec3& value) const
{
	glUniform3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec3(const string& name, float x, float y, float z) const
{
	glUniform3f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z);
}
// ------------------------------------------------------------------------
void Shader::setVec4(const string& name, const glm::vec4& value) const
{
	glUniform4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, &value[0]);
}
void Shader::setVec4(const string& name, float x, float y, float z, float w) const
{
	glUniform4f(glGetUniformLocation(ShaderProgramID, name.c_str()), x, y, z, w);
}
// ------------------------------------------------------------------------
void Shader::setMat2(const string& name, const glm::mat2& mat) const
{
	glUniformMatrix2fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat3(const string& name, const glm::mat3& mat) const
{
	glUniformMatrix3fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
// ------------------------------------------------------------------------
void Shader::setMat4(const string& name, const glm::mat4& mat) const
{
	glUniformMatrix4fv(glGetUniformLocation(ShaderProgramID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}
