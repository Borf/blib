#pragma once

#include <list>

namespace blib
{
	namespace gl
	{
		class GlInitRegister
		{
			static std::list<GlInitRegister*> registeredObjects;
		public:
			static void initRegisteredObjects();
			GlInitRegister();
			~GlInitRegister();
			virtual void initGl() = 0;
		};
	}
}