#pragma once

#include <string>
#include <list>
#include <map>

#include <blib/Shader.h>
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

#include "glm/glm.hpp"
namespace blib
{
	namespace gl
	{
		class Shader : public blib::Shader
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
		public:
			std::map<std::string, GLuint> uniformLocations;
			std::list<SubShader*> shaders;
			GLuint programId;
	 
			GLuint getUniformLocation(const std::string &name);
			std::string id;
		public:
			Shader();
			Shader(std::string vertex, std::string fragment);
			
			virtual ~Shader();
			virtual void init(){};
			void use();
			void link();


			char** uniformNames;

			enum Uniforms
			{
				modelmatrix,
				cameraMatrix,
				matrix,
			};

		protected:
			void doUniform(const std::string &name,			const glm::mat4& value);
			void doUniform(const std::string &name,			const glm::mat3& value);
			void doUniform(const std::string &name,			const float& value);
			void doUniform(const std::string &name,			const int& value);
			void doUniform(const std::string &name,			const glm::vec4& value);
			void doUniform(const std::string &name,			const glm::vec3& value);
			void doUniform(const std::string &name,			const glm::vec2& value);

		};
	}
}
