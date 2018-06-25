#pragma once

#include <glad/glad.h> // 包含glad来获取所有的必须OpenGL头文件

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

class Shader
{
public:

	Shader(const GLchar* vertexPath, const GLchar* fragmentPath, const GLchar* geometryPath = nullptr);
	~Shader();

	inline unsigned int GetID() { return m_ID; }

	// 使用/激活程序
	void use();
	// uniform工具函数
	void setBool(const std::string &name, bool value) const;
	void setInt(const std::string &name, int value) const;
	void setFloat(const std::string &name, float value) const;
	inline void setVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	inline void setVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_ID, name.c_str()), x, y);
	}
	// ------------------------------------------------------------------------
	inline void setVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	inline void setVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_ID, name.c_str()), x, y, z);
	}
	// ------------------------------------------------------------------------
	inline void setVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_ID, name.c_str()), 1, &value[0]);
	}
	inline void setVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_ID, name.c_str()), x, y, z, w);
	}
	// ------------------------------------------------------------------------
	inline void setMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	// ------------------------------------------------------------------------
	inline void setMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}

private:

	void checkCompileErrors(GLuint shader, std::string type);
	unsigned int m_ID;
};

