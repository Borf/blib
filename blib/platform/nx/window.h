#pragma once

#include <blib/IWindow.h>
#include <blib/KeyListener.h>

#include <vector>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <nn/vi.h>
#include <GL/glew.h>


namespace blib
{
	class App;
	namespace platform
	{
		namespace nx
		{
			class Window : public blib::IWindow
			{
			private:
				float ratio = 1;
				App* app;
				std::vector<long> clicks;

				EGLDisplay s_Display;
				EGLSurface s_Surface;
				EGLContext s_Context;

				nn::vi::NativeWindowHandle  s_NativeWindowHandle;
				nn::vi::Display*            s_pDisplay;
				nn::vi::Layer*              s_pLayer;
				GLuint vao;

			protected:
			public:
				Window(App* app);
				virtual ~Window();

				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual bool makeCurrent();
				virtual void unmakeCurrent();

				virtual void touchDownEvent(unsigned long id, int x, int y);
				virtual void touchUpEvent(unsigned long id, int x, int y);
				virtual void touchMoveEvent(unsigned long id, int x, int y);

				virtual void keyDownEvent(blib::Key key);
				virtual void tick();

				void setWidth(int width);
				void setHeight(int height);

			};
		}
	}
}
