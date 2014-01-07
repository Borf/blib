#pragma once

#include <blib/IWindow.h>
#include <blib/KeyListener.h>

#include <EGL/egl.h>


namespace blib
{
	namespace platform
	{
		namespace android
		{
			class Window : public blib::IWindow
			{
			private:
			    EGLDisplay display;
			    EGLSurface surface;
			    EGLContext context;
			protected:
			public:
				Window();
				virtual ~Window();

				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();

				virtual void touchDownEvent(int x, int y);
				virtual void touchUpEvent(int x, int y);
				virtual void touchMoveEvent(int x, int y);

				virtual void keyDownEvent(blib::Key key);


			};
		}
	}
}
