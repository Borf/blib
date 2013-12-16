#pragma once

#include <blib/Window.h>
#include <blib/util/NotCopyable.h>

namespace blib
{
	namespace gl
	{
		class Window : public blib::Window, blib::util::NotCopyable
		{
		public:

#ifdef WIN32
			HGLRC hrc;
#endif

			Window();
			virtual ~Window();
			virtual void create(int icon, std::string title);
			virtual void tick();
		};
	}
}
