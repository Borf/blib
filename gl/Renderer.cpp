#include <blib/gl/Renderer.h>

#include <gl/glew.h>
#include <blib/gl/Shader.h>
#include <blib/RenderState.h>
#include <blib/Texture.h>
#include <blib/gl/FBO.h>
#include <blib/gl/VBO.h>
#include <blib/util/Log.h>

using blib::util::Log;

namespace blib
{
	namespace gl
	{
		void Renderer::flush()
		{
			int totalVerts = 0;

			for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
			{
				Render* r = toRender[1-activeLayer][i];
				if(r->command == Render::Clear)
				{
					if(r->renderState.activeFbo != NULL)
						r->renderState.activeFbo->bind();
					else
						glBindFramebuffer(GL_FRAMEBUFFER, 0);

					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

					glClearColor(	((blib::Renderer::RenderClear*)r)->color.r, 
									((blib::Renderer::RenderClear*)r)->color.g, 
									((blib::Renderer::RenderClear*)r)->color.b, 
									((blib::Renderer::RenderClear*)r)->color.a);
					int bits = 0;
					if(((Renderer::RenderClear*)r)->clearColor)
						bits |= GL_COLOR_BUFFER_BIT;
					if(((Renderer::RenderClear*)r)->clearDepth)
						bits |= GL_DEPTH_BUFFER_BIT;
					if(((Renderer::RenderClear*)r)->clearStencil)
						bits |= GL_STENCIL_BUFFER_BIT;

					glClear(bits);
					continue;
				}
				else if(r->command == Render::SetVbo)
				{
					r->perform(vertices[1-activeLayer]);
				}
				else if(r->command == Render::DrawTriangles || r->command == Render::DrawLines)
				{
					r->renderState.activeShader->use();
					r->renderState.activeShader->setState(r->state);

					if(r->renderState.activeFbo != NULL)
						r->renderState.activeFbo->bind();
					else
						glBindFramebuffer(GL_FRAMEBUFFER, 0);

					if(r->renderState.stencilTestEnabled)
					{
						glEnable(GL_STENCIL_TEST);
						glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
						glStencilMask(0xFF);

						if(r->renderState.stencilWrite)
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


					if(r->renderState.activeTexture[0])
					{
						r->renderState.activeTexture[0]->use();
					}
					if(r->renderState.activeTexture[1])
					{
						glActiveTexture(GL_TEXTURE1);
						r->renderState.activeTexture[1]->use();
						glActiveTexture(GL_TEXTURE0);
					}

					if(r->renderState.blendEnabled)
					{
						glEnable(GL_BLEND);
						glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
					}
					else
						glDisable(GL_BLEND);


					if(r->renderState.activeVbo)
						r->renderState.activeVbo->bind();
					else
						glBindBuffer(GL_ARRAY_BUFFER, 0);
					r->setVertexAttributes(vertices[1-activeLayer]);

					totalVerts += r->vertexCount();
					if(r->command == Render::DrawTriangles)
						glDrawArrays(GL_TRIANGLES, 0, r->vertexCount());
					else//if(r->command == Renderer::DrawLines)
						glDrawArrays(GL_LINES, 0, r->vertexCount());
				}

			}
		//	Log::out<<toRender[1-activeLayer].size()<< " render commands, "<<totalVerts<<" vertices"<<Log::newline;
			//delete takes up a lot of time here, let's see if we can reuse the list of vertices
			for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
				delete toRender[1-activeLayer][i];
			toRender[1-activeLayer].clear();
		}

	}
}

