#include <blib/gl/Renderer.h>

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

#include <glm/gtc/matrix_transform.hpp>

#include <blib/gl/Shader.h>
#include <blib/RenderState.h>
#include <blib/Texture.h>
#include <blib/gl/FBO.h>
#include <blib/gl/VBO.h>
#include <blib/gl/VIO.h>
#include <blib/util/Log.h>
#include <blib/gl/Vertex.h>
#include <blib/App.h>
#include <blib/math/Ray.h>
#include <fstream>

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
            static GLint oldFbo = -1;
			if(oldFbo == -1)
				glGetIntegerv(GL_FRAMEBUFFER_BINDING, &oldFbo);
            
			int totalVerts = 0;
			RenderState* lastRenderState = NULL;

			for(size_t i = 0; i < toRender[1-activeLayer].size(); i++)
			{
				Render* r = toRender[1-activeLayer][i];
				if(r->command == Render::Clear)
				{
                    if(r->renderState.activeFbo == NULL)
						glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
					else
						r->renderState.activeFbo->bind();

					glDisable(GL_SCISSOR_TEST);
					glClearStencil(0);
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

					lastRenderState = NULL;// &r->renderState; // TODO: apply all settings of this renderstate
					continue;
				}
				else if (r->command == Render::SetViewPort)
				{
					glViewport(((RenderSetViewPort*)r)->left, ((RenderSetViewPort*)r)->top, ((RenderSetViewPort*)r)->width, ((RenderSetViewPort*)r)->height);
					height = ((RenderSetViewPort*)r)->height;
				}
				else if (r->command == Render::SetVbo)
				{
					r->perform(vertices[1 - activeLayer]);
				}
				else if (r->command == Render::SetVboSub)
				{
					r->perform(vertices[1 - activeLayer]);
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
				else if (r->command == Render::SetVio)
				{
					r->perform(NULL);
				}
				else if (r->command == Render::Unproject)
				{
					RenderUnproject* ru = (RenderUnproject*)r;
					int Viewport[4];
					glGetIntegerv(GL_VIEWPORT, Viewport);


					ru->mousePosition.y = Viewport[3] - ru->mousePosition.y;


					if (ru->target)
					{
						float winZ;
						glReadPixels((int)ru->mousePosition.x, (int)ru->mousePosition.y, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ); //TODO: maybe need to do windowheight-mouse3d.y
						glm::vec4 ret = glm::vec4(glm::unProject(glm::vec3(ru->mousePosition, winZ), ru->modelMatrix, ru->projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3])), winZ);

						glm::vec4* target = ru->target;
						app->runLater<int>([ret, target](int) {*target = ret; }, 0);
					}
					if (ru->ray)
					{
						glm::vec3 retNear = glm::unProject(glm::vec3(ru->mousePosition, 0), ru->modelMatrix, ru->projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3]));
						glm::vec3 retFar = glm::unProject(glm::vec3(ru->mousePosition, 1), ru->modelMatrix, ru->projectionMatrix, glm::vec4(Viewport[0], Viewport[1], Viewport[2], Viewport[3]));

						blib::math::Ray* ray = ru->ray;
						app->runLater<int>([retNear, retFar, ray](int) { ray->origin = retNear; ray->dir = glm::normalize(retFar - retNear); ray->calcSign();  }, 0);
					}


				}
				else if (r->command == Render::SaveFbo)
				{
#ifdef BLIB_WIN
					glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
					((RenderSaveFbo*)r)->fbo->use();
					int w, h;
					glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &w);
					glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &h);
					char* data = new char[3*w*h];
					glGetTexImage(GL_TEXTURE_2D, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
					int xa = w % 256;
					int xb = (w - xa) / 256; 
					int ya = h % 256;
					int yb = (h - ya) / 256;//assemble the header
					unsigned char header[18] = { 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, (unsigned char)xa, (unsigned char)xb, (unsigned char)ya, (unsigned char)yb, 24, 0 };
					// write header and data to file
					std::fstream File(((RenderSaveFbo*)r)->filename.c_str(), std::ios::out | std::ios::binary);
					File.write(reinterpret_cast<char *>(header), sizeof(char) * 18);
					File.write(reinterpret_cast<char *>(data), sizeof(char)*(w*h*3));
					File.close();

					delete[] data;
#endif
				}
				else if(r->command == Render::DrawTriangles || r->command == Render::DrawLines || r->command == Render::DrawPoints || r->command == Render::DrawIndexedTriangles)
				{
					if (!lastRenderState || lastRenderState->activeShader != r->renderState.activeShader)
						r->renderState.activeShader->use();

					r->renderState.activeShader->setState(r->shaderState);


                    if (!lastRenderState || lastRenderState->depthTest != r->renderState.depthTest) {
						if(r->renderState.depthTest)
							glEnable(GL_DEPTH_TEST);
						else
                            glDisable(GL_DEPTH_TEST);
                    }

                    if (!lastRenderState || lastRenderState->activeFbo != r->renderState.activeFbo) {
						if(r->renderState.activeFbo == NULL)
							glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
						else
                            r->renderState.activeFbo->bind();
                    }


                    if (!lastRenderState || lastRenderState->stencilTestEnabled != r->renderState.stencilTestEnabled || lastRenderState->stencilWrite != r->renderState.stencilWrite) {
						if(r->renderState.stencilTestEnabled)
						{
							glEnable(GL_STENCIL_TEST);
							glStencilMask(0xFF);

							if(r->renderState.stencilWrite)
							{
								glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
								glStencilFunc(GL_NEVER, 1, 0xFF);
								glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
							}
							else
							{
								glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
								glStencilFunc(GL_EQUAL, 0, 0xFF);
								glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
							}
						}
						else
						{
							glDisable(GL_STENCIL_TEST);
							glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
                        }
                    }

#if !defined(BLIB_IOS) && !defined(BLIB_ANDROID) && !defined(BLIB_EMSCRIPTEN)
					if (!lastRenderState || r->renderState.renderStyle != lastRenderState->renderStyle)
						glPolygonMode(GL_FRONT_AND_BACK, r->renderState.renderStyle == RenderState::WIREFRAME ? GL_LINE : GL_FILL);
#endif
                    
					if (!lastRenderState || lastRenderState->activeTexture[0] != r->renderState.activeTexture[0])
						if (r->renderState.activeTexture[0])
							r->renderState.activeTexture[0]->use();
					if (!lastRenderState || lastRenderState->activeTexture[1] != r->renderState.activeTexture[1])
						if (r->renderState.activeTexture[1])
						{
							glActiveTexture(GL_TEXTURE1);
							r->renderState.activeTexture[1]->use();
							glActiveTexture(GL_TEXTURE0);
						}
					if (!lastRenderState || lastRenderState->activeTexture[2] != r->renderState.activeTexture[2])
						if (r->renderState.activeTexture[2])
						{
						glActiveTexture(GL_TEXTURE2);
						r->renderState.activeTexture[2]->use();
						glActiveTexture(GL_TEXTURE0);
						}


					if (!lastRenderState || 
						r->renderState.blendEnabled != lastRenderState->blendEnabled ||
						r->renderState.srcBlendColor != lastRenderState->srcBlendColor ||
                        r->renderState.srcBlendAlpha != lastRenderState->srcBlendAlpha ||
                        r->renderState.dstBlendColor != lastRenderState->dstBlendColor ||
                        r->renderState.dstBlendAlpha != lastRenderState->dstBlendAlpha) {
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
                        else {
                            glDisable(GL_BLEND);
                        }

                    }

					if (!lastRenderState ||
						r->renderState.scissor != lastRenderState->scissor ||
                        memcmp(r->renderState.scissorArea, lastRenderState->scissorArea, sizeof(int)*4) != 0) {
						if (r->renderState.scissor)
						{
							glEnable(GL_SCISSOR_TEST);
							glScissor(r->renderState.scissorArea[0], height - r->renderState.scissorArea[1] - r->renderState.scissorArea[3], r->renderState.scissorArea[2], r->renderState.scissorArea[3]);
						}
                        else {
                            glDisable(GL_SCISSOR_TEST);
                        }
                    }

                    if (!lastRenderState || lastRenderState->activeVbo != r->renderState.activeVbo) {
                        if(r->renderState.activeVbo) {
							r->renderState.activeVbo->bind();
                        } else {
                            glBindBuffer(GL_ARRAY_BUFFER, 0);
                        }
                    }

                    if (!lastRenderState || lastRenderState->activeVio != r->renderState.activeVio) {
						if (r->renderState.activeVio)
							r->renderState.activeVio->bind();
						else
                            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                    }


					if (!lastRenderState || lastRenderState->cullFaces != r->renderState.cullFaces)
					{
                        if (r->renderState.cullFaces == RenderState::CullFaces::NONE) {
							glDisable(GL_CULL_FACE);
                        } else {
							glEnable(GL_CULL_FACE);
							if (r->renderState.cullFaces == RenderState::CullFaces::CCW)
								glCullFace(GL_BACK);
							else
								glCullFace(GL_FRONT);
						}
					}



					r->setVertexAttributes(enabledVertexAttributes, vertices[1-activeLayer]);
					totalVerts += r->vertexCount();

					int start = 0;
					if(r->renderState.activeVbo)
						start = ((RenderBlock<blib::VertexP3T2>*)r)->vertexStart;	//TODO

					if(r->command == Render::DrawTriangles)
						glDrawArrays(GL_TRIANGLES, start, r->vertexCount());
					else if (r->command == Render::DrawLines)
					{
						float w = ((RenderBlock<blib::Vertex>*)r)->lineThickness;
						if (w > 0 && r->vertexCount() > 0)
						{
							glLineWidth(w);
							glDrawArrays(GL_LINES, start, r->vertexCount());
						}
					}
					else if(r->command == Render::DrawPoints)
					{
#if !defined(BLIB_ANDROID) && !defined(BLIB_IOS)
						glEnable(GL_POINT_SPRITE);
						glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
                        glPointSize(10);
#endif
						glDrawArrays(GL_POINTS, start, r->vertexCount());
					}
					else if (r->command == Render::DrawIndexedTriangles)
					{
						//TODO: get the unsigned short from the renderblock
						//TODO: get rid of these casts here
						if(r->renderState.activeVio->elementSize == sizeof(unsigned short))
							glDrawElements(GL_TRIANGLES, ((RenderBlock<blib::VertexP3T2>*)r)->count, GL_UNSIGNED_SHORT, (void*)(((RenderBlock<blib::VertexP3T2>*)r)->vertexStart * sizeof(unsigned short)));
						else
							glDrawElements(GL_TRIANGLES, ((RenderBlock<blib::VertexP3T2>*)r)->count, GL_UNSIGNED_INT, (void*)(((RenderBlock<blib::VertexP3T2>*)r)->vertexStart * sizeof(unsigned int)));
					}
					lastRenderState = &r->renderState;
				}


			}
//			Log::out<<toRender[1-activeLayer].size()<< " render commands, "<<totalVerts<<" vertices, verts"<<vertexIndex[1-activeLayer]<<Log::newline;
			//delete takes up a lot of time here, let's see if we can reuse the list of vertices

			clearCommands();
#ifdef WIN32
//TODO            glBindFramebuffer(GL_FRAMEBUFFER, oldFbo);
#endif
		}

	}
}

