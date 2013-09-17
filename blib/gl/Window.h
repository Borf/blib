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

			HGLRC hrc;

			Window();
			virtual ~Window();
			virtual void create(int icon, std::string title);
			virtual void swapBuffers();
			virtual void tick();
		};
	}
}