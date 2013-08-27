#include <blib/gl/Renderer.h>

#include <gl/glew.h>
#include <blib/gl/Shader.h>
#include <blib/RenderState.h>
#include <blib/Texture.h>
#include <blib/gl/FBO.h>

namespace blib
{
	namespace gl
	{
		void Renderer::flush()
		{

			for(size_t i = 0; i < toRender.size(); i++)
			{
				if(toRender[i]->command == Render::Clear)
				{
					glClearColor(	((blib::Renderer::RenderClear*)toRender[i])->color.r, 
									((blib::Renderer::RenderClear*)toRender[i])->color.g, 
									((blib::Renderer::RenderClear*)toRender[i])->color.b, 
									((blib::Renderer::RenderClear*)toRender[i])->color.a);
					int bits = 0;
					if(((Renderer::RenderClear*)toRender[i])->clearColor)
						bits |= GL_COLOR_BUFFER_BIT;
					if(((Renderer::RenderClear*)toRender[i])->clearDepth)
						bits |= GL_DEPTH_BUFFER_BIT;
					if(((Renderer::RenderClear*)toRender[i])->clearStencil)
						bits |= GL_STENCIL_BUFFER_BIT;

					glClear(bits);
					continue;
				}
				else
				{
					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);

					toRender[i]->renderState.activeShader->use();
					toRender[i]->renderState.activeShader->setState(toRender[i]->state);

					if(toRender[i]->renderState.activeFbo != NULL)
						toRender[i]->renderState.activeFbo->bind();
					else
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

					if(toRender[i]->renderState.stencilTestEnabled)
					{
						glEnable(GL_STENCIL_TEST);
						glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
						glStencilMask(0xFF);

						if(toRender[i]->renderState.stencilWrite)
						{
							glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
							glStencilFunc(GL_NEVER, 1, 0xFF);
						}
						else
						{
							glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
							glStencilFunc(GL_EQUAL, 0, 0xFF);
						}
					}
					else
					{
						glDisable(GL_STENCIL_TEST);
						glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
					}


					if(toRender[i]->renderState.activeTexture[0])
						toRender[i]->renderState.activeTexture[0]->use();
					if(toRender[i]->renderState.activeTexture[1])
						toRender[i]->renderState.activeTexture[1]->use();

					glBindBuffer(GL_ARRAY_BUFFER, 0);
					glEnableVertexAttribArray(0);
					glEnableVertexAttribArray(1);
					toRender[i]->setVertexAttributes();
					glBindBuffer(GL_ARRAY_BUFFER, 0);
//					glEnableVertexAttribArray(0);
//					glEnableVertexAttribArray(1);
					glDrawArrays(GL_TRIANGLES, 0, toRender[i]->vertexCount());
				}

			}

			for(size_t i = 0; i < toRender.size(); i++)
				delete toRender[i];
			toRender.clear();
		}

	}
}

