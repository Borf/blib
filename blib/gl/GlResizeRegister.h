#pragma once

#include <list>

namespace blib
{
	namespace gl
	{
		class GlResizeRegister
		{
			static std::list<GlResizeRegister*> registeredObjects;
		public:
			static void ResizeRegisteredObjects(int width, int height);
			GlResizeRegister();
			virtual ~GlResizeRegister();
			virtual void resizeGl(int width, int height) = 0;
		};
	}
}