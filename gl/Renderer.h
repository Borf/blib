#pragma once


#include <blib/Renderer.h>


namespace blib
{
	namespace gl
	{
		class Renderer : public blib::Renderer
		{
		public:
			void flush();
		};
	}
}