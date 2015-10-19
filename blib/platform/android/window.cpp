#include <blib/platform/android/window.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/util/Log.h>
#include <blib/Util.h>
#include <blib/App.h>

using blib::util::Log;

#include <android_native_app_glue.h>

extern android_app* androidApp;


using blib::util::Log;

namespace blib
{
	namespace platform
	{
		namespace android
		{
			Window::Window(App* app)
			{
				this->app = app;
			}
			Window::~Window()
			{
			}


			void Window::create(int icon, std::string title)
			{
				surface = 0;
			    const EGLint attribs[] = {
			    		EGL_RENDERABLE_TYPE, //EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			            EGL_OPENGL_ES2_BIT,
			            EGL_BLUE_SIZE, 8,
			            EGL_GREEN_SIZE, 8,
			            EGL_RED_SIZE, 8,
			            EGL_NONE
			    };
			    EGLint w, h, dummy, format;
			    EGLint numConfigs;
			    EGLConfig config;

			    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
			    eglInitialize(display, 0, 0);
			    eglChooseConfig(display, attribs, &config, 1, &numConfigs);
			    eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);
			    ANativeWindow_setBuffersGeometry(androidApp->window, 0, 0, format);


			    EGLint ContextAttributes[] = {
			    	EGL_CONTEXT_CLIENT_VERSION,
			        2,      // selects OpenGL ES 2.0, set to 1 to select OpenGL ES 1.1
			    	EGL_NONE
			    };
			    surface = eglCreateWindowSurface(display, config, androidApp->window, NULL);
			    context = eglCreateContext(display, config, EGL_NO_CONTEXT, ContextAttributes);

			    eglQuerySurface(display, surface, EGL_WIDTH, &w);
				eglQuerySurface(display, surface, EGL_HEIGHT, &h);

				Log::out<<"Surface of "<<w<<", "<<h<<Log::newline;
				if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
						Log::out<<"Unable to eglMakeCurrent"<<Log::newline;
						return;
				}

				width = w;
				height = h;

				Log::out<<"Done creating egl window, surface "<<(int)surface<<Log::newline;
			}

			void Window::swapBuffers()
			{
				eglSwapBuffers(display, surface);
			}


			bool Window::makeCurrent()
			{
				if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
					Log::out<<"Unable to eglMakeCurrent"<<Log::newline;
						return false;
				}
				return true;
			}
			void Window::unmakeCurrent()
			{
				if (eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE) {
					Log::out<<"Unable to eglUnMakeCurrent"<<Log::newline;
						return;
				}
			}

			int id = 1;
			void Window::touchDownEvent(int x, int y)
			{
				int clickCount = 1;
				clicks.push_back(blib::util::tickcount());
				int i = clicks.size() - 2;
				while (i >= 0 && clicks[i] > clicks[i + 1] - 200)
				{
					i--;
					clickCount++;
				}
				if (clickCount < clicks.size())
					clicks.erase(clicks.begin(), clicks.begin() + clicks.size() - clickCount);

				for (std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseDown(x, y, MouseListener::Left, clickCount);

				for (Touch& t : app->touches)
				{
					if (t.id == 0)
					{
						t.id = id;
						t.position.x = x;
						t.position.y = y;
						break;
					}
				}
			}

			void Window::touchUpEvent(int x, int y)
			{
				for (std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseUp(x, y, MouseListener::Left, 1);
				for (Touch& t : app->touches)
				{
					if (t.id == id)
					{
						t.id = 0;
						t.position.x = 0;
						t.position.y = 0;
						break;
					}
				}
			}

			void Window::touchMoveEvent(int x, int y)
			{
				for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseMove(x,y,MouseListener::Left);
			}

			void Window::keyDownEvent(blib::Key key)
			{
				for (std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseMove(x, y, MouseListener::Left);

				for (Touch& t : app->touches)
				{
					if (t.id == id)
					{
						t.position.x = x;
						t.position.y = y;
						break;
					}
				}
			}



		}
	}
}
