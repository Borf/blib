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
			std::list<SubShader*> shaders;
			GLuint programId;
			std::string id;
		public:
			Shader();
			Shader(std::string vertex, std::string fragment);
			
			virtual ~Shader();
			virtual void init(){};
			void use();
			void link();

			std::string createcallstack;
			char** uniformNames;

			enum Uniforms
			{
				modelmatrix,
				cameraMatrix,
				matrix,
			};

		protected:
		};
	}
}
