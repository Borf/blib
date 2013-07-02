#pragma once

#include <blib/Window.h>

namespace blib
{
	namespace gl
	{
		class Window : public blib::Window
		{
		public:
			Window();
			~Window();
			virtual void create();
			virtual void swapBuffers();
			virtual void tick();

		private:
			Window(const Window& other) { throw "not copyable!"; };
			Window& operator = (Window& other) { throw "not copyable!"; };

		};
	}
}