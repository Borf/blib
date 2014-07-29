#include "Shader.h"
#include <blib/util/Log.h>
#include <blib/gl/Shader.h>
using blib::util::Log;


#include <blib/config.h>

#if defined(BLIB_IOS)
#include <OpenGLES/ES2/gl.h>
#include <OpenGLES/ES2/glext.h>
#elif defined(BLIB_ANDROID)
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#ifdef WIN32
#include <GL/wglew.h>
#endif
#endif

#include <string.h>

namespace blib
{
	void Shader::setState( char* state )
	{
	/*	for(std::map<std::string,	float>::iterator it = state.floatValues.begin(); it != state.floatValues.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	int>::iterator it = state.intValues.begin(); it != state.intValues.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec2>::iterator it = state.vec2Values.begin(); it != state.vec2Values.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec3>::iterator it = state.vec3Values.begin(); it != state.vec3Values.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	glm::vec4>::iterator it = state.vec4Values.begin(); it != state.vec4Values.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	glm::mat3>::iterator it = state.mat3Values.begin(); it != state.mat3Values.end(); it++)
			doUniform(it->first, it->second);
		for(std::map<std::string,	glm::mat4>::iterator it = state.mat4Values.begin(); it != state.mat4Values.end(); it++)
			doUniform(it->first, it->second);*/

		for (int i = 0; i < uniformCount; i++)
		{
			if (uniforms[i])
			{
				int location = uniforms[i]->id;
				switch (uniforms[i]->type)
				{
				case Float:
					glUniform1f(location, (float&)state[uniforms[i]->index]);
					break;
				case Int:
					glUniform1i(location, (int&)state[uniforms[i]->index]);
					break;
				case Mat4:
					if (activeUniformData[uniforms[i]->index - 1] != state[uniforms[i]->index - 1])
					{
						glUniformMatrix4fv(location, 1, 0, &((float&)state[uniforms[i]->index]));
						activeUniformData[uniforms[i]->index - 1] = state[uniforms[i]->index - 1];
					}
					break;
				case Mat3:
					if (activeUniformData[uniforms[i]->index - 1] != state[uniforms[i]->index - 1])
					{
						glUniformMatrix3fv(location, 1, 0, &((float&)state[uniforms[i]->index]));
						activeUniformData[uniforms[i]->index - 1] = state[uniforms[i]->index - 1];
					}
					break;
				case Vec2:
					glUniform2f(location, (float&)state[uniforms[i]->index], (float&)state[uniforms[i]->index + 4]);
					break;
				case Vec4:
					glUniform4f(location, (float&)state[uniforms[i]->index], (float&)state[uniforms[i]->index + 4], (float&)state[uniforms[i]->index + 8], (float&)state[uniforms[i]->index+12]);
					break;
				default:
					Log::out << "Error in uniform type" << Log::newline;
				}

			}
		}

	}

	void Shader::bindAttributeLocation( std::string name, int index )
	{
		attributes[name] = index;
	}

	void Shader::initFromData( std::string vertexShader, std::string fragmentShader )
	{
		this->vertexShader = vertexShader;
		this->fragmentShader = fragmentShader;
	}

	Shader::Shader()
	{
		uniformCount = 0;
		uniformSize = 0;
		uniformData = NULL;
		activeUniformData = NULL;
		for (int i = 0; i < 16; i++)
			uniforms[i] = NULL;
	}

	void Shader::finishUniformSetup()
	{
		uniformData = new char[uniformSize];
		activeUniformData = new char[uniformSize];
		memset(activeUniformData, 0, uniformSize);
		memset(activeUniformData, 0, uniformSize);
	}


#ifdef STUPIDIPHONE
	template<>
	void Shader::Uniform::set(char* data, const glm::mat4& value)
	{
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				((float&)data[index + (4 * y + x)*sizeof(float)]) = value[y][x];
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::mat3& value)
	{
		for (int x = 0; x < 3; x++)
			for (int y = 0; y < 3; y++)
				((float&)data[index + (3 * y + x)*sizeof(float)]) = value[y][x];
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::vec2& value)
	{
		for (int i = 0; i < 2; i++)
				((float&)data[index + i*sizeof(float)]) = value[i];
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::vec3& value)
	{
		for (int i = 0; i < 3; i++)
			((float&)data[index + i*sizeof(float)]) = value[i];
	}	template<>
		void Shader::Uniform::set(char* data, const glm::vec4& value)
	{
			for (int i = 0; i < 4; i++)
				((float&)data[index + i*sizeof(float)]) = value[i];
	}
#endif
}
