#pragma once

#include <blib/IWindow.h>
#include <blib/KeyListener.h>

#include <EGL/egl.h>
#include <vector>


namespace blib
{
	class App;
	namespace platform
	{
		namespace android
		{
			class Window : public blib::IWindow
			{
			private:
				App* app;
				EGLDisplay display;
			    EGLSurface surface;
			    EGLContext context;
				std::vector<long> clicks;
			protected:
			public:
				Window(App* app);
				virtual ~Window();

				void tick() {};
				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual bool makeCurrent();
				virtual void unmakeCurrent();

				virtual void touchDownEvent(int x, int y);
				virtual void touchUpEvent(int x, int y);
				virtual void touchMoveEvent(int x, int y);

				virtual void keyDownEvent(blib::Key key);


			};
		}
	}
}
