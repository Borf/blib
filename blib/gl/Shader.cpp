#include "Shader.h"
#include <blib/util/FileSystem.h>
#include <blib/Util.h>
#include <blib/util/Log.h>
using blib::util::Log;
#include <glm/gtc/type_ptr.hpp>

namespace blib
{
	namespace gl
	{
		Shader::Shader()
		{
			programId = 0;
			this->createcallstack = blib::util::callstack();
		}

		Shader::Shader(std::string vertex, std::string fragment)
		{
			programId = 0;
			this->vertexShaders.push_back(vertex);
			this->fragmentShaders.push_back(fragment);
//			this->createcallstack = blib::util::callstack();
            if(vertexShaders.empty() || fragmentShaders.empty())
            {
                Log::out<<"a shader is still empty"<<Log::newline;
            }
		}




		Shader::~Shader(void)
		{
			if(programId != 0)
				glDeleteProgram(programId);
			for(std::list<SubShader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
				delete (*it);
			shaders.clear();
		}
		

		//called from renderer
		void Shader::use()
		{
			if(programId == 0)
			{
				if(vertexShaders.empty() || fragmentShaders.empty())
				{
					Log::out<<"a shader is still empty"<<Log::newline;
				}

				programId = glCreateProgram();
				if(programId == 0)
					return;

				for (const std::string &vertexShader : vertexShaders)
					shaders.push_back(SubShader::fromData(vertexShader.c_str(), SubShader::Vertex));
				for (const std::string &fragmentShader : fragmentShaders)
					shaders.push_back(SubShader::fromData(fragmentShader.c_str(), SubShader::Fragment));

				for (std::list<SubShader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
					if (*it == NULL)
						Log::out << "Error creating shader...\n" << createcallstack << Log::newline;


				for(std::list<SubShader*>::iterator it = shaders.begin(); it != shaders.end(); it++)
					(*it)->attach(programId);
				
				if(attributes.empty())
					Log::out<<"No attributes for shader!"<<Log::newline;
				for(std::map<std::string, int>::iterator it = attributes.begin(); it != attributes.end(); it++)
					glBindAttribLocation(programId, it->second, it->first.c_str());
				link();


				int total = -1;
				glGetProgramiv(programId, GL_ACTIVE_UNIFORMS, &total);
				for (int i = 0; i < total; ++i)  {
					int name_len = -1, num = -1;
					GLenum type = GL_ZERO;
					char name[100];
					glGetActiveUniform(programId, GLuint(i), sizeof(name) - 1,
						&name_len, &num, &type, name);
					name[name_len] = 0;
					//Log::out << "Found uniform " << name << Log::newline;
				}

				glUseProgram(programId);
				for (int i = 0; i < uniformCount; i++)
				{
					if (uniforms[i])
					{
						if (uniforms[i]->type == Struct)
						{ //todo: add more recursive way of doing this
							StructUniform* uniformStruct = (StructUniform*)uniforms[i];

							for (size_t ii = 0; ii < uniformStruct->members.size(); ii++)
							{
								uniformStruct->members[ii]->id = glGetUniformLocation(programId, uniformStruct->members[ii]->name.c_str());
								if (uniformStruct->members[ii]->id == -1)
									Log::out << "Error: uniform " << uniformStruct->members[ii]->name << " not found in shader" << Log::newline;
							}
						}
						else if (uniforms[i]->type == Array)
						{//todo: add more recursive way of doing this
							ArrayUniform* uniformStruct = (ArrayUniform*)uniforms[i];

							for (size_t ii = 0; ii < uniformStruct->members.size(); ii++)
							{
								uniformStruct->members[ii]->id = glGetUniformLocation(programId, uniformStruct->members[ii]->name.c_str());
								if (uniformStruct->members[ii]->id == -1)
									Log::out << "Error: uniform " << uniformStruct->members[ii]->name << " not found in shader" << Log::newline;
							}

						}
						else
						{
							uniforms[i]->id = glGetUniformLocation(programId, uniforms[i]->name.c_str());
							if (uniforms[i]->id == -1)
								Log::out << "Error: uniform " << uniforms[i]->name << " not found in shader" << Log::newline;
						}
					}
				}
			}
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
#if defined(BLIB_GL_FULL)
			std::string data = blib::util::FileSystem::getData("assets/shaders/gl/" + filename);
#elif defined(BLIB_GL_ES)
			std::string data = blib::util::FileSystem::getData("assets/shaders/gles/" + filename);
#else
#error	You need to define a GL version...
#endif
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
			//glDeleteShader(shaderId);
		}

		void Shader::SubShader::attach(GLuint programId)
		{
			glAttachShader(programId, shaderId);
		}


		/*void Shader::bindFragLocation( std::string name, int position )
		{
			if(glBindFragDataLocation)
				glBindFragDataLocation(programId, position, name.c_str());
		}*/

	}
}
