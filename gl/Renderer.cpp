#include <blib/gl/Renderer.h>

#include <gl/glew.h>
#include <blib/gl/Shader.h>
#include <blib/RenderState.h>

namespace blib
{
	namespace gl
	{
		void Renderer::flush()
		{

			for(size_t i = 0; i < toRender.size(); i++)
			{
				toRender[i]->renderState.activeShader->use();
				toRender[i]->renderState.activeShader->setState(toRender[i]->state);

				toRender[i]->setVertexAttributes();
				glDrawArrays(GL_TRIANGLES, 0, toRender[i]->vertexCount());

			}




			for(size_t i = 0; i < toRender.size(); i++)
				delete toRender[i];
			toRender.clear();
		}
	}
}

