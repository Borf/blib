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
            static void ResizeRegisteredObjects(int width, int height, int offx = 0, int offy = 0);
			GlResizeRegister();
			virtual ~GlResizeRegister();
			virtual void resizeGl(int width, int height, int offx, int offy) = 0;

        
            static int width;
            static int height;
            static int offx;
            static int offy;
        
        };
	}
}
