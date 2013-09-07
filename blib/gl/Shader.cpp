#include "Shader.h"
#include <blib/util/FileSystem.h>
#include <blib/util/Log.h>
using blib::util::Log;
#include <glm/gtc/type_ptr.hpp>

namespace blib
{
	namespace gl
	{
		Shader::Shader()
		{
			programId = glCreateProgram();
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


		Shader::SubShader::SubShader(ShaderType shaderType)
		{
			shaderId = glCreateShader(shaderType == Vertex ? GL_VERTEX_SHADER : GL_FRAGMENT_SHADER);
		}

		Shader::SubShader* Shader::SubShader::fromFile(std::string filename, ShaderType shaderType)
		{
			SubShader* subShader = new SubShader(shaderType);
			std::string data = blib::util::FileSystem::getData(filename);
			const char* cdata = data.c_str();
			glShaderSource(subShader->shaderId, 1, &cdata,NULL);
			glCompileShader(subShader->shaderId);
			int status;
			glGetShaderiv(subShader->shaderId, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE)
			{
				int length, charsWritten;
				glGetShaderiv(subShader->shaderId, GL_INFO_LOG_LENGTH, &length);
				char* infolog = new char[length+1];
				memset(infolog, 0, length+1);
				glGetShaderInfoLog(subShader->shaderId, length, &charsWritten, infolog);
				Log::out<<"Error compiling shader "<<filename<<":\n"<<infolog<<Log::newline;
				delete[] infolog;
				delete subShader;
				return NULL;
			}
			return subShader;
		}
		Shader::SubShader* Shader::SubShader::fromData(const char* data, ShaderType shaderType)
		{
			SubShader* subShader = new SubShader(shaderType);
			glShaderSource(subShader->shaderId, 1, &data,NULL);
			glCompileShader(subShader->shaderId);
			int status;
			glGetShaderiv(subShader->shaderId, GL_COMPILE_STATUS, &status);
			if(status == GL_FALSE)
			{
				int length, charsWritten;
				glGetShaderiv(subShader->shaderId, GL_INFO_LOG_LENGTH, &length);
				char* infolog = new char[length+1];
				memset(infolog, 0, length+1);
				glGetShaderInfoLog(subShader->shaderId, length, &charsWritten, infolog);
				Log::out<<"Error compiling shader from memory:\n"<<infolog<<Log::newline;
				delete[] infolog;
				delete subShader;
				return NULL;
			}
			return subShader;
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




		GLuint Shader::getUniformLocation( const std::string &name )
		{
			if(uniformLocations.find(name) == uniformLocations.end())
				uniformLocations[name] = glGetUniformLocation(programId, name.c_str());
			return uniformLocations[name];
		}




		void Shader::doUniform(const std::string &name, const glm::mat4& value )
		{
			int location = getUniformLocation(name);
			glUniformMatrix4fv(location, 1, 0, glm::value_ptr(value));
		}

		void Shader::doUniform(const std::string &name, const glm::mat3& value )
		{
			int location = getUniformLocation(name);
			glUniformMatrix3fv(location, 1, 0, glm::value_ptr(value));
		}

		void Shader::doUniform(const std::string &name, const int &value )
		{
			int location = getUniformLocation(name);
			glUniform1i(location, value);
		}

		void Shader::doUniform(const std::string &name, const float &value )
		{
			int location = getUniformLocation(name);
			glUniform1f(location, value);
		}

		void Shader::doUniform(const std::string &name, const glm::vec4& value )
		{
			int location = getUniformLocation(name);
			glUniform4f(location, value.x, value.y, value.z, value.w);
		}

		void Shader::doUniform(const std::string &name, const glm::vec3& value )
		{
			int location = getUniformLocation(name);
			glUniform3f(location, value.x, value.y, value.z);
		}
		void Shader::doUniform(const std::string &name, const glm::vec2& value )
		{
			int location = getUniformLocation(name);
			glUniform2f(location, value.x, value.y);
		}

	}
}
