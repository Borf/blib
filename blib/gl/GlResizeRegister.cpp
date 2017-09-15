#include "GlResizeRegister.h"

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
