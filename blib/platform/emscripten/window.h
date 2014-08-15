#pragma once

#include <blib/IWindow.h>

#include <blib/util/Log.h>
using blib::util::Log;

#include <EGL/egl.h>

namespace blib
{
	class App;
	namespace platform
	{
		namespace emscripten
		{
			class Window : public blib::IWindow
			{
			private:
				App* app;
				EGLDisplay  eglDisplay;
				EGLContext  eglContext;
				EGLSurface  eglSurface;
				EGLNativeWindowType hWnd;

			public:
				Window(App* app);
				virtual ~Window();

				std::vector<long> clicks;


				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
                virtual void tick() { }
			};
		}
	}
}
