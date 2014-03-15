#include <blib/gl/Renderer.h>

#ifdef ANDROID
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#else
#include <GL/glew.h>
#endif

#include <blib/gl/Shader.h>
#include <blib/RenderState.h>
#include <blib/Texture.h>
#include <blib/gl/FBO.h>
#include <blib/gl/VBO.h>
#include <blib/util/Log.h>
#include <blib/gl/Vertex.h>

using blib::util::Log;

namespace blib
{
	namespace gl
	{
		int toGlEnum(blib::RenderState::BlendOptions option)
		{
			if(option == RenderState::ZERO)
				return GL_ZERO;
			if(option == RenderState::ONE)
				return GL_ONE;
			if(option == RenderState::SRC_ALPHA)
				return GL_SRC_ALPHA;
			if(option == RenderState::ONE_MINUS_SRC_ALPHA)
				return GL_ONE_MINUS_SRC_ALPHA;
			
			return -1;
		}


		void Renderer::flush()
		{
			int totalVerts = 0;

			RenderState* lastRenderState = NULL;

			for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
			{
				Render* r = toRender[1-activeLayer][i];
				if(r->command == Render::Clear)
				{
					if(r->renderState.activeFbo.empty())
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
					else
					{
						r->renderState.activeFbo.top()->bind();
					}

					glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

					glClearColor(	((blib::Renderer::RenderClear*)r)->color.x, 
									((blib::Renderer::RenderClear*)r)->color.y, 
									((blib::Renderer::RenderClear*)r)->color.z, 
									((blib::Renderer::RenderClear*)r)->color.w);
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
				else if (r->command == Render::SetViewPort)
				{
					glViewport(0,0,((RenderSetViewPort*)r)->width, ((RenderSetViewPort*)r)->height);
				}
				else if(r->command == Render::SetVbo)
				{
					r->perform(vertices[1-activeLayer]);
				}
				else if(r->command == Render::SetSubTexture)
				{
					RenderSetSubTexture* rst = (RenderSetSubTexture*)r;
					rst->texture->setSubImage(rst->x, rst->y, rst->width, rst->height, rst->data);
					delete[] rst->data;
				}
				else if (r->command == Render::SetShaderState)
				{
//					RenderSetShaderState* rsss = (RenderSetShaderState*)r;
//					rsss->shader->use();
//					rsss->shader->setState(rsss->state);
				}
				else if(r->command == Render::DrawTriangles || r->command == Render::DrawLines || r->command == Render::DrawPoints)
				{
					if (!lastRenderState)
						r->renderState.activeShader->use();
					if (lastRenderState->activeShader != r->renderState.activeShader)
						r->renderState.activeShader->use();

					r->renderState.activeShader->setState(r->shaderState);


					if(r->renderState.depthTest)
						glEnable(GL_DEPTH_TEST);
					else
						glDisable(GL_DEPTH_TEST);

					if(r->renderState.activeFbo.empty())
						glBindFramebuffer(GL_FRAMEBUFFER, 0);
					else
						r->renderState.activeFbo.top()->bind();

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

					if(r->renderState.renderStyle == RenderState::WIREFRAME)
						glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
					else
						glPolygonMode( GL_FRONT_AND_BACK, GL_FILL);

					if(r->renderState.activeTexture[0])
					{
						if (!lastRenderState)
							r->renderState.activeShader->use();
						if (lastRenderState->activeTexture[0] != r->renderState.activeTexture[0])
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

						int srcBlendColor = toGlEnum(r->renderState.srcBlendColor);
						int srcBlendAlpha = toGlEnum(r->renderState.srcBlendAlpha);
						int dstBlendColor = toGlEnum(r->renderState.dstBlendColor);
						int dstBlendAlpha = toGlEnum(r->renderState.dstBlendAlpha);
						glBlendFuncSeparate(srcBlendColor, dstBlendColor, srcBlendAlpha, dstBlendAlpha);
						//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);
					}
					else
						glDisable(GL_BLEND);

					if(r->renderState.activeVbo)
						r->renderState.activeVbo->bind();
					else
						glBindBuffer(GL_ARRAY_BUFFER, 0);
					r->setVertexAttributes(enabledVertexAttributes, vertices[1-activeLayer]);

					totalVerts += r->vertexCount();

					int start = 0;
					if(r->renderState.activeVbo)
						start = ((RenderBlock<blib::VertexP3T2>*)r)->vertexStart;	//TODO

					if(r->command == Render::DrawTriangles)
						glDrawArrays(GL_TRIANGLES, start, r->vertexCount());
					else if(r->command == Render::DrawLines)
						glDrawArrays(GL_LINES, start, r->vertexCount());
					else if(r->command == Render::DrawPoints)
					{
#ifndef ANDROID
						glEnable(GL_POINT_SPRITE);
						glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif
						glDrawArrays(GL_POINTS, start, r->vertexCount());
					}
				}

				lastRenderState = &r->renderState;

			}
		//	Log::out<<toRender[1-activeLayer].size()<< " render commands, "<<totalVerts<<" vertices"<<Log::newline;
			//delete takes up a lot of time here, let's see if we can reuse the list of vertices
		//	for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
		//		delete toRender[1-activeLayer][i];
			allocators[1 - activeLayer].clear(); // deletes all objects
			toRender[1-activeLayer].clear();
		}

	}
}

