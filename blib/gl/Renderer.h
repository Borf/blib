#pragma once


#include <blib/Renderer.h>


namespace blib
{
	namespace gl
	{
		class Renderer : public blib::Renderer
		{
			int height;
		public:
			void flush();
		};
	}
}