#include "Box2DDebug.h"
#include <glm/glm.hpp>
#include <blib/LineBatch.h>
#include <blib/Renderer.h>
#include <blib/Util.h>
#include <blib/math/Rectangle.h>

#include <glm/gtc/matrix_transform.hpp>
#include <blib/Math.h>

namespace blib
{
	Box2DDebug::Box2DDebug() : cullRectangle(glm::vec2(0,0),glm::vec2(0,0))
	{
		alpha = 0.5f;
		lineBatch = NULL;
		renderer = NULL;
		cull = false;
	}

	void Box2DDebug::init( LineBatch* lineBatch, Renderer* renderer )
	{
		this->lineBatch = lineBatch;
		this->renderer = renderer;
	}


	void Box2DDebug::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{
		for(int i = 0; i < vertexCount; i++)
		{
			int ii = (i+1)%vertexCount;
			if(!cull || cullRectangle.contains(glm::vec2(vertices[i].x, vertices[i].y)) || cullRectangle.contains(glm::vec2(vertices[ii].x, vertices[ii].y)))
				lineBatch->draw(glm::vec2(vertices[i].x, vertices[i].y), glm::vec2(vertices[ii].x, vertices[ii].y), glm::vec4(color.r, color.g, color.b, 1));
		}
	}

	void Box2DDebug::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{//todo: fill
		/*glm::vec4 clr(color.r, color.g, color.b, alpha);
		std::vector<VertexP2C4> verts;
		for(int i = 1; i < vertexCount-1; i++)
		{
			verts.push_back(VertexP2C4((b2Vec2)vertices[0], glm::vec4(color.r, color.g, color.b, alpha)));
			for(int ii = i; ii < i+2; ii++)
			{
				verts.push_back(VertexP2C4((b2Vec2)vertices[ii], glm::vec4(color.r, color.g, color.b, alpha)));
			}
		}
		for(int i = 0; i < vertexCount; i++)
		{
			int ii = (i+1)%vertexCount;
			lineBatch->draw((b2Vec2)vertices[i], (b2Vec2)vertices[ii], glm::vec4(color.r, color.g, color.b, 1));
		}

		renderer->drawTriangles(verts, lineBatch->renderState);*/
		
		for (int i = 0; i < vertexCount; i++)
		{
			int ii = (i + 1) % vertexCount;
			if (!cull || cullRectangle.contains(glm::vec2(vertices[i].x, vertices[i].y)) || cullRectangle.contains(glm::vec2(vertices[ii].x, vertices[ii].y)))
				lineBatch->draw(glm::vec2(vertices[i].x, vertices[i].y), glm::vec2(vertices[ii].x, vertices[ii].y), glm::vec4(color.r, color.g, color.b, 1));
		}
		
	}

	void Box2DDebug::DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color )
	{
		if (cull && !cullRectangle.contains(center))
			return;
        float inc = blib::math::pif/16;
		glm::vec2 c =(b2Vec2)center;
        for(float i = 0; i < 2*blib::math::pif; i+=inc)
			lineBatch->draw(c + util::fromAngle(i)*radius, c + util::fromAngle(i+inc)*radius, glm::vec4(color.r, color.g, color.b, 1));
	}

	void Box2DDebug::DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
	{//todo: fill
	/*	glm::vec4 clr(color.r, color.g, color.b, alpha);
		std::vector<VertexP2C4> verts;
		glm::vec2 c =(b2Vec2)center;

		float inc = (float)(M_PI/16);
		for(float i = 0; i < 2*M_PI; i+=inc)
		{
			verts.push_back(VertexP2C4(c + util::fromAngle(i)*radius, clr));
			verts.push_back(VertexP2C4(c + util::fromAngle(i+inc)*radius, clr));
			verts.push_back(VertexP2C4(c, clr));
			lineBatch->draw(c + util::fromAngle(i)*radius, c + util::fromAngle(i+inc)*radius, glm::vec4(color.r, color.g, color.b, 1));
		}
		renderer->renderState.activeShader = lineBatch->shader;
		lineBatch->shader->setUniform(LineBatch::Uniforms::matrix, lineBatch->matrix);
		renderer->drawTriangles(verts);*/

		if (cull && !cullRectangle.contains(center))
			return;
		float inc = blib::math::pif / 16;
		glm::vec2 c = (b2Vec2)center;
        for (float i = 0; i < 2 * blib::math::pif; i += inc)
			lineBatch->draw(c + util::fromAngle(i)*radius, c + util::fromAngle(i + inc)*radius, glm::vec4(color.r, color.g, color.b, 1));




	}

	void Box2DDebug::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
	{
		if (!cull || cullRectangle.contains(glm::vec2(p1.x, p1.y)) || cullRectangle.contains(glm::vec2(p2.x, p2.y)))
			lineBatch->draw((b2Vec2)p1, (b2Vec2)p2, glm::vec4(color.r, color.g, color.b, 1));
	}

	void Box2DDebug::DrawTransform( const b2Transform& xf )
	{

	}

	void Box2DDebug::DrawPoint(const b2Vec2& p, float32 size, const b2Color& color)
	{

	}

}

