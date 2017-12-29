#include "ShaderManager.h"



ShaderManager::ShaderManager(const GLchar* vertexPath, const GLchar* fragmentPath)
{
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;

	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try
	{
		//open shader files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		
		//transfer data into file stream
		std::stringstream vShaderStream, fShaderStream;
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		
		//close files;
		vShaderFile.close();
		fShaderFile.close();
		
		//transfer stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
	}
	
	//transfer STL string into char* string
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	//create a vertex shader object, bind it to source, compile it
	unsigned int vertexShader;
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vShaderCode, NULL);
	glCompileShader(vertexShader);

	//create a fragment shader object, bind it to source, compile it
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fShaderCode, NULL);
	glCompileShader(fragmentShader);

	//link two shaders to shader program
	shader_program_id = glCreateProgram();
	glAttachShader(shader_program_id, vertexShader);
	glAttachShader(shader_program_id, fragmentShader);
	glLinkProgram(shader_program_id);

	//delete shaders after using the program
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}


ShaderManager::~ShaderManager()
{

}

void ShaderManager::use()
{
	glUseProgram(shader_program_id);
}

void ShaderManager::setBool(const std::string &name, bool value) const
{
	glUniform1i(glGetUniformLocation(shader_program_id, name.c_str()), (int)value);
}

void ShaderManager::setInt(const std::string &name, int value) const 
{
	glUniform1i(glGetUniformLocation(shader_program_id, name.c_str()), value);
}

void ShaderManager::setFloat(const std::string &name, float value) const
{
	glUniform1f(glGetUniformLocation(shader_program_id, name.c_str()), value);
}
