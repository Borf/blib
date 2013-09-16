#include "Box2DDebug.h"
#include <glm/glm.hpp>
#include <blib/LineBatch.h>
#include <blib/Util.h>

#define _USE_MATH_DEFINES
#include <math.h>

namespace blib
{
	Box2DDebug::Box2DDebug()
	{
		alpha = 0.5f;
	}

	void Box2DDebug::init( LineBatch* lineBatch )
	{
		this->lineBatch = lineBatch;
	}


	void Box2DDebug::DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{
		for(int i = 0; i < vertexCount; i++)
		{
			int ii = (i+1)%vertexCount;
			lineBatch->draw(glm::vec2(vertices[i].x, vertices[i].y), glm::vec2(vertices[ii].x, vertices[ii].y), glm::vec4(color.r, color.g, color.b, alpha));
		}
	}

	void Box2DDebug::DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color )
	{//todo: fill
		for(int i = 0; i < vertexCount; i++)
		{
			int ii = (i+1)%vertexCount;
			lineBatch->draw((b2Vec2)vertices[i], (b2Vec2)vertices[ii], glm::vec4(color.r, color.g, color.b, alpha));
		}
	}

	void Box2DDebug::DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color )
	{
		float inc = (float)(M_PI/8);
		glm::vec2 c =(b2Vec2)center;
		for(float i = 0; i < 2*M_PI; i+=inc)
			lineBatch->draw(c + util::fromAngle(i), c + util::fromAngle(i+inc), glm::vec4(color.r, color.g, color.b, alpha));
	}

	void Box2DDebug::DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color )
	{//todo: fill
		float inc = (float)(M_PI/8);
		glm::vec2 c =(b2Vec2)center;
		for(float i = 0; i < 2*M_PI; i+=inc)
			lineBatch->draw(c + util::fromAngle(i), c + util::fromAngle(i+inc), glm::vec4(color.r, color.g, color.b, alpha));
	}

	void Box2DDebug::DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color )
	{
		lineBatch->draw((b2Vec2)p1, (b2Vec2)p2, glm::vec4(color.r, color.g, color.b, alpha));
	}

	void Box2DDebug::DrawTransform( const b2Transform& xf )
	{

	}

}

