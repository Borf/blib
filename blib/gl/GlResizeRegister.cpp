#include "GlResizeRegister.h"

#include <blib/util/Log.h>
using blib::util::Log;

namespace blib
{
	namespace gl
	{
        std::list<GlResizeRegister*> GlResizeRegister::registeredObjects;
        int GlResizeRegister::width = 1024;
        int GlResizeRegister::height = 768;
        int GlResizeRegister::offy = 0;
        int GlResizeRegister::offx = 0;
        
        
        void GlResizeRegister::ResizeRegisteredObjects(int width, int height, int offx, int offy)
		{
			Log::out << "Resize register, resizing to " << width << "x" << height << ", offset " << offx << "," << offy << Log::newline;
            GlResizeRegister::width = width;
            GlResizeRegister::height = height;
            GlResizeRegister::offx = offx;
            GlResizeRegister::offy = offy;
			for(std::list<GlResizeRegister*>::iterator it = registeredObjects.begin(); it != registeredObjects.end(); it++)
				(*it)->resizeGl(width, height, offx, offy);
		}

		GlResizeRegister::GlResizeRegister()
		{
			registeredObjects.push_back(this);
		}
		GlResizeRegister::~GlResizeRegister()
		{
			registeredObjects.remove(this);
		}


	}
}
