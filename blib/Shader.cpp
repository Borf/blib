#include "Shader.h"
#include <blib/util/Log.h>
#include <blib/gl/Shader.h>
using blib::util::Log;


#include <blib/config.h>
#include <blib/util/FileSystem.h>

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
	void setUniform_(Shader::Uniform* uniform, char* state, char* activeUniformData)
	{
		int location = uniform->id;
		switch (uniform->type)
		{
		case Shader::Float:
			glUniform1f(location, (float&)state[uniform->index]);
			break;
		case Shader::Int:
			glUniform1i(location, (int&)state[uniform->index]);
			break;
		case Shader::Mat4:
#ifndef BLIB_EMSCRIPTEN
			if (activeUniformData[uniform->index - 1] != state[uniform->index - 1])
#endif
			{
				glUniformMatrix4fv(location, 1, 0, &((float&)state[uniform->index]));
				activeUniformData[uniform->index - 1] = state[uniform->index - 1];
			}
			break;
		case Shader::Mat3:
#ifndef BLIB_EMSCRIPTEN
			if (activeUniformData[uniform->index - 1] != state[uniform->index - 1])
#endif
			{
				glUniformMatrix3fv(location, 1, 0, &((float&)state[uniform->index]));
				activeUniformData[uniform->index - 1] = state[uniform->index - 1];
			}
			break;
		case Shader::Vec2:
			glUniform2f(location, (float&)state[uniform->index], (float&)state[uniform->index + 4]);
			break;
		case Shader::Bool:
			glUniform1i(location, (bool&)state[uniform->index] ? 1 : 0);
			break;
		case Shader::Vec3:
			glUniform3f(location, (float&)state[uniform->index], (float&)state[uniform->index + 4], (float&)state[uniform->index + 8]);
			break;
		case Shader::Vec4:
			glUniform4f(location, (float&)state[uniform->index], (float&)state[uniform->index + 4], (float&)state[uniform->index + 8], (float&)state[uniform->index + 12]);
			break;
		case Shader::Struct:
		{
			Shader::StructUniform* uniformStruct = (Shader::StructUniform*)uniform;
			for (size_t i = 0; i < uniformStruct->members.size(); i++)
			{
				setUniform_(uniformStruct->members[i], state, activeUniformData);
			}
		}
			break;
		case Shader::Array:
		{
			Shader::ArrayUniform* uniformStruct = (Shader::ArrayUniform*)uniform;
			for (size_t i = 0; i < uniformStruct->members.size(); i++)
			{
				setUniform_(uniformStruct->members[i], state, activeUniformData);
			}
		}
			break;
		default:
			Log::out << "Error in uniform type" << Log::newline;
		}

	}


	//TODO: move this to the opengl shader class
	void Shader::setState( char* state )
	{
		for (int i = 0; i < uniformCount; i++)
		{
			if (uniforms[i])
			{
				setUniform_(uniforms[i], state, activeUniformData);
			}
		}

	}

	void Shader::bindAttributeLocation( std::string name, int index )
	{
		attributes[name] = index;
	}

	void Shader::initFromData( std::string vertexShader, std::string fragmentShader )
	{
		this->vertexShaders.push_back(vertexShader);
		this->fragmentShaders.push_back(fragmentShader);
	}

	void Shader::addVertexShader(const std::string &fileName)
	{
#if defined(BLIB_GL_FULL)
		vertexShaders.push_back(blib::util::FileSystem::getData("assets/shaders/gl/" + fileName + ".vert"));
#elif defined(BLIB_GL_ES)
		vertexShaders.push_back(blib::util::FileSystem::getData("assets/shaders/gl/es" + fileName + ".vert"));
#else
#error	You need to define a GL version...
#endif
	}


	Shader::Shader() : Resource("Shader")
	{
		uniformCount = 0;
		uniformSize = 0;
		uniformData = NULL;
		activeUniformData = NULL;
		for (int i = 0; i < 16; i++)
			uniforms[i] = NULL;
	}

	Shader::~Shader()
	{
		if (uniformData)
			delete[] uniformData;
		if (activeUniformData)
			delete[] activeUniformData;
		for (int i = 0; i < 16; i++)
			delete uniforms[i];
	}


	void Shader::finishUniformSetup()
	{
		uniformData = new char[uniformSize];
		activeUniformData = new char[uniformSize];
		memset(uniformData, 0, uniformSize);
		memset(activeUniformData, 0, uniformSize);
	}



#ifdef STUPIDIPHONE
	template<>
	void Shader::Uniform::set(char* data, const glm::mat4& value)
	{
        data[index - 1]++;
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				memcpy(data + (index + (4 * y + x)*sizeof(float)), &value[y][x], sizeof(float));
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::mat3& value)
	{
        data[index - 1]++;
		for (int x = 0; x < 3; x++)
			for (int y = 0; y < 3; y++)
				memcpy(data + (index + (3 * y + x)*sizeof(float)), &value[y][x], sizeof(float));
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::vec2& value)
	{
        data[index - 1]++;
		for (int i = 0; i < 2; i++)
            memcpy(data + (index + i*sizeof(float)), &value[i], sizeof(float));
	}
	template<>
	void Shader::Uniform::set(char* data, const glm::vec3& value)
	{
        data[index - 1]++;
		for (int i = 0; i < 3; i++)
            memcpy(data + (index + i*sizeof(float)), &value[i], sizeof(float));
	}

	template<>
	void Shader::Uniform::set(char* data, const glm::vec4& value)
	{
        data[index - 1]++;
			for (int i = 0; i < 4; i++)
                memcpy(data + (index + i*sizeof(float)), &value[i], sizeof(float));
	}


	template<>
	glm::mat4 Shader::Uniform::get(char* data)
	{
		glm::mat4 ret;
		for (int x = 0; x < 4; x++)
			for (int y = 0; y < 4; y++)
				ret[y][x] = ((float&)data[index + (4 * y + x)*sizeof(float)]);
		return ret;
	}
	template<>
	glm::mat3 Shader::Uniform::get(char* data)
	{
		glm::mat3 ret;
		for (int x = 0; x < 3; x++)
			for (int y = 0; y < 3; y++)
				ret[y][x] = ((float&)data[index + (3 * y + x)*sizeof(float)]);
		return ret;
	}

	template<>
	glm::vec2 Shader::Uniform::get(char* data)
	{
		glm::vec2 ret;
		for (int i = 0; i < 2; i++)
			ret[i] = ((float&)data[index + i*sizeof(float)]);
		return ret;
	}
	template<>
	glm::vec3 Shader::Uniform::get(char* data)
	{
		glm::vec3 ret;
		for (int i = 0; i < 3; i++)
			ret[i] = ((float&)data[index + i*sizeof(float)]);
		return ret;
	}
	template<>
	glm::vec4 Shader::Uniform::get(char* data)
	{
		glm::vec4 ret;
		for (int i = 0; i < 4; i++)
			ret[i] = ((float&)data[index + i*sizeof(float)]);
		return ret;
	}

#endif
}
