#pragma once

#include <string>
#include <list>
#include <map>

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif
#include "glm/glm.hpp"
namespace blib
{
	namespace gl
	{
		class Shader
		{
		protected:
			class SubShader
			{
			public:
				enum ShaderType
				{
					Vertex,
					Fragment
				};
			protected:
				GLuint shaderId;
				SubShader(ShaderType shaderType);
			public:
				static SubShader* fromFile(std::string filename, ShaderType shaderType);
				static SubShader* fromData(const char* filename, ShaderType shaderType);
				~SubShader();
				void attach(GLuint programId);
			};

			std::map<std::string, GLuint> uniformLocations;
			std::list<SubShader*> shaders;
			GLuint programId;
	 
			GLuint getUniformLocation(std::string name);

			Shader();
		public:
			template<class T>
			static T* fromFile(std::string vertex, std::string fragment)
			{
				T* shader = new T();
				shader->shaders.push_back(SubShader::fromFile(vertex, SubShader::Vertex));
				shader->shaders.push_back(SubShader::fromFile(fragment, SubShader::Fragment));
				for(std::list<SubShader*>::iterator it = shader->shaders.begin(); it != shader->shaders.end(); it++)
					(*it)->attach(shader->programId);
				shader->init();
				return shader;
			}
			template<class T>
			static T* fromData(const char* vertex, const char* fragment)
			{
				T* shader = new T();
				shader->shaders.push_back(SubShader::fromData(vertex, SubShader::Vertex));
				shader->shaders.push_back(SubShader::fromData(fragment, SubShader::Fragment));
				for(std::list<SubShader*>::iterator it = shader->shaders.begin(); it != shader->shaders.end(); it++)
					(*it)->attach(shader->programId);
				shader->init();
				return shader;
			}
			virtual ~Shader();
			virtual void init(){};
			void use();
			void link();

			void bindAttributeLocation(std::string name, int position);
			//void bindFragLocation( std::string name, int position );

			void setUniform(std::string name,			const glm::mat4& value);
			void setUniform(std::string name,			const glm::mat3& value);
			void setUniform(std::string name,			const float& value);
			void setUniform(std::string name,			const int& value);
			void setUniform(std::string name,			const glm::vec4& value);
			void setUniform(std::string name,			const glm::vec3& value);
			void setUniform(std::string name,			const glm::vec2& value);

		};
	}
}
