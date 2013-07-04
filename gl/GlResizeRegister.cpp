#include "GlResizeRegister.h"

namespace blib
{
	namespace gl
	{

		std::list<GlResizeRegister*> GlResizeRegister::registeredObjects;
		void GlResizeRegister::ResizeRegisteredObjects(int width, int height)
		{
			for(std::list<GlResizeRegister*>::iterator it = registeredObjects.begin(); it != registeredObjects.end(); it++)
				(*it)->resizeGl(width, height);
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