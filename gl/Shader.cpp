#include "Shader.h"
#include <pewpew/fs.h>
#include "Log.h"

#include <glm/gtc/type_ptr.hpp>


Shader::Shader(std::string vertexShader, std::string fragmentShader)
{
	programId = glCreateProgram();
	shaders.push_back(new SubShader(vertexShader, SubShader::Vertex));
	shaders.push_back(new SubShader(fragmentShader, SubShader::Fragment));

	for(std::list<SubShader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
		(*it)->attach(programId);
}


Shader::~Shader(void)
{
	glDeleteProgram(programId);
	
	for(std::list<SubShader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
		delete (*it);
	shaders.clear();
	
	
}

void Shader::use()
{
	glUseProgram(programId);
}

void Shader::link()
{
	glLinkProgram(programId);
	/*
	int length, charsWritten;
	glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &length);
	char* infolog = new char[length+1];
	memset(infolog, 0, length+1);
	glGetProgramInfoLog(programId, length, &charsWritten, infolog);
	logger<<"Error compiling Program :\n"<<infolog<<Log::newline;
	delete[] infolog;*/


}




Shader::SubShader::SubShader(std::string filename, ShaderType shaderType)
{
	shaderId = glCreateShader(shaderType == Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
	std::string data = File::getData(filename);
	const char* cdata = data.c_str();
	glShaderSource(shaderId, 1, &cdata,NULL);
	glCompileShader(shaderId);
	int status;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &status);
	if(status == GL_FALSE)
	{
		int length, charsWritten;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &length);
		char* infolog = new char[length+1];
		memset(infolog, 0, length+1);
		glGetShaderInfoLog(shaderId, length, &charsWritten, infolog);
		logger<<"Error compiling shader "<<filename<<":\n"<<infolog<<Log::newline;
		delete[] infolog;
	}
}


Shader::SubShader::~SubShader()
{
	glDeleteShader(shaderId);
}

void Shader::SubShader::attach(GLuint programId)
{
	glAttachShader(programId, shaderId);
}


void Shader::bindAttributeLocation(std::string name, int position)
{
	glBindAttribLocation(programId, position, name.c_str());
}

/*void Shader::bindFragLocation( std::string name, int position )
{
	if(glBindFragDataLocation)
		glBindFragDataLocation(programId, position, name.c_str());
}*/




GLuint Shader::getUniformLocation( std::string name )
{
	if(uniformLocations.find(name) == uniformLocations.end())
		uniformLocations[name] = glGetUniformLocation(programId, name.c_str());
	return uniformLocations[name];
}




void Shader::setUniform( std::string name, const glm::mat4& value )
{
	int location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, 0, glm::value_ptr(value));
}

void Shader::setUniform( std::string name, const int &value )
{
	int location = getUniformLocation(name);
	glUniform1i(location, value);
}

void Shader::setUniform( std::string name, const float &value )
{
	int location = getUniformLocation(name);
	glUniform1f(location, value);
}

void Shader::setUniform( std::string name, const glm::vec4& value )
{
	int location = getUniformLocation(name);
	glUniform4f(location, value.x, value.y, value.z, value.w);
}

void Shader::setUniform( std::string name, const glm::vec3& value )
{
	int location = getUniformLocation(name);
	glUniform3f(location, value.x, value.y, value.z);
}
void Shader::setUniform( std::string name, const glm::vec2& value )
{
	int location = getUniformLocation(name);
	glUniform2f(location, value.x, value.y);
}
