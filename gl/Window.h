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
			Window();
			~Window();
			virtual void create();
			virtual void swapBuffers();
			virtual void tick();
		};
	}
}