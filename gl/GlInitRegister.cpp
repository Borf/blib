#include "GlInitRegister.h"

namespace blib
{
	namespace gl
	{
		std::list<GlInitRegister*> GlInitRegister::registeredObjects;
		void GlInitRegister::initRegisteredObjects()
		{
			for(std::list<GlInitRegister*>::iterator it = registeredObjects.begin(); it != registeredObjects.end(); it++)
				(*it)->initGl();
		}

		GlInitRegister::GlInitRegister()
		{
			registeredObjects.push_back(this);
		}
		GlInitRegister::~GlInitRegister()
		{
			registeredObjects.remove(this);
		}


	}
}