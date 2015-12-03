#pragma once

#include <Box2D/Box2D.h>
#include <blib/util/Singleton.h>
#include <blib/math/Rectangle.h>

namespace blib
{
	class LineBatch;
	class Renderer;

	class Box2DDebug : public b2Draw, public util::Singleton<Box2DDebug>
	{
		LineBatch* lineBatch;
		Renderer* renderer;
		float alpha;
	public:
		bool cull;
		blib::math::Rectangle cullRectangle;

		Box2DDebug();
		void init(LineBatch* lineBatch, Renderer* renderer);

		virtual void DrawPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color );
		virtual void DrawSolidPolygon( const b2Vec2* vertices, int32 vertexCount, const b2Color& color );
		virtual void DrawCircle( const b2Vec2& center, float32 radius, const b2Color& color );
		virtual void DrawSolidCircle( const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color );
		virtual void DrawSegment( const b2Vec2& p1, const b2Vec2& p2, const b2Color& color );
		virtual void DrawTransform( const b2Transform& xf );
	};

}