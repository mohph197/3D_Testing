#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:
	Shader(const char* VertexPath, const char* FragmentPath);
	unsigned int GetProgramID();
	void use();
	void setInt(const string& name, int value) const;
	void setFloat(const string& name, float value) const;
	void setBool(const string& name, bool value) const;
	void setVec2(const string& name, const glm::vec2& value) const;
	void setVec2(const string& name, float x, float y) const;
	void setVec3(const string& name, const glm::vec3& value) const;
	void setVec3(const string& name, float x, float y, float z) const;
	void setVec4(const string& name, const glm::vec4& value) const;
	void setVec4(const string& name, float x, float y, float z, float w) const;
	void setMat2(const string& name, const glm::mat2& mat) const;
	void setMat3(const string& name, const glm::mat3& mat) const;
	void setMat4(const string& name, const glm::mat4& mat) const;
private:
	unsigned int ShaderProgramID;
	unsigned int vertexShaderID, fragmentShaderID;
	ifstream vertexShaderFile, fragmentShaderFile;
	stringstream vertexCodeStream, fragmentCodeStream;
	string vertexCodeS, fragmentCodeS;
	const char* vertexCode, * fragmentCode;
	int success;
	char infoLog[1024];
	void CheckErrors(unsigned int &ID, string type);
};

#endif