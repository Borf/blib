#pragma once

#include <blib/Window.h>
#include <blib/util/NotCopyable.h>

namespace blib
{
	class App;

	namespace gl
	{
		class Window : public blib::Window, blib::util::NotCopyable
		{
		public:

#ifdef WIN32
			HGLRC hrc;
#endif

			Window(App* app);
			virtual ~Window();
			virtual void create(int icon, std::string title);
			virtual void tick();
			virtual void onResize(int width, int height, int offX, int offY);
		};
	}
}
