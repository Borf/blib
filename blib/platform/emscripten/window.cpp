#include <blib/platform/ios/window.h>

#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/App.h>
#include <blib/Util.h>

#include  <X11/Xlib.h>
#include  <X11/Xatom.h>
#include  <X11/Xutil.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>

using blib::util::Log;

static Display* x_display = NULL;


#define FALSE 0
#define TRUE (!FALSE)

namespace blib
{
	namespace platform
	{
		namespace emscripten
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
				int flags = 0;
				EGLint attribList[] =
				{
					EGL_RED_SIZE, 5,
					EGL_GREEN_SIZE, 6,
					EGL_BLUE_SIZE, 5,
					EGL_ALPHA_SIZE, 8,
					EGL_DEPTH_SIZE, 8,
					EGL_STENCIL_SIZE, 8,
					EGL_SAMPLE_BUFFERS, 0,
					EGL_NONE
				};

				width = 1024;
				height = 768;
				//createwindow
				::Window root;
				XSetWindowAttributes swa;
				XSetWindowAttributes  xattr;
				Atom wm_state;
				XWMHints hints;
				XEvent xev;
				EGLConfig ecfg;
				EGLint num_config;
				::Window win;

				/*
				* X11 native display initialization
				*/

				x_display = XOpenDisplay(NULL);
				if (x_display == NULL)
				{
					Log::out << "Could not open display" << Log::newline;
					return;
				}

				root = DefaultRootWindow(x_display);

				swa.event_mask = ExposureMask | PointerMotionMask | KeyPressMask;
				win = XCreateWindow(
					x_display, root,
					0, 0, width, height, 0,
					CopyFromParent, InputOutput,
					CopyFromParent, CWEventMask,
					&swa);

				xattr.override_redirect = FALSE;
				XChangeWindowAttributes(x_display, win, CWOverrideRedirect, &xattr);

				hints.input = TRUE;
				hints.flags = InputHint;
				XSetWMHints(x_display, win, &hints);

				// make the window visible on the screen
				XMapWindow(x_display, win);
				XStoreName(x_display, win, title.c_str());

				// get identifiers for the provided atom name strings
				wm_state = XInternAtom(x_display, "_NET_WM_STATE", FALSE);

				memset(&xev, 0, sizeof(xev));
				xev.type = ClientMessage;
				xev.xclient.window = win;
				xev.xclient.message_type = wm_state;
				xev.xclient.format = 32;
				xev.xclient.data.l[0] = 1;
				xev.xclient.data.l[1] = FALSE;
				XSendEvent(
					x_display,
					DefaultRootWindow(x_display),
					FALSE,
					SubstructureNotifyMask,
					&xev);

				hWnd = (EGLNativeWindowType)win;
				////create context
				EGLint numConfigs;
				EGLint majorVersion;
				EGLint minorVersion;
				EGLDisplay display;
				EGLContext context;
				EGLSurface surface;
				EGLConfig config;
				EGLint contextAttribs[] = { EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE, EGL_NONE };

				// Get Display
				display = eglGetDisplay((EGLNativeDisplayType)x_display);
				if (display == EGL_NO_DISPLAY)
				{
					Log::out << "Unable to make eglGetDisplay" << Log::newline;
					return;
				}

				// Initialize EGL
				if (!eglInitialize(display, &majorVersion, &minorVersion))
				{
					Log::out << "Unable to make eglInitialize" << Log::newline;
					return ;
				}

				// Get configs
				if (!eglGetConfigs(display, NULL, 0, &numConfigs))
				{
					Log::out << "Unable to make eglGetConfigs" << Log::newline;
					return;
				}

				// Choose config
				if (!eglChooseConfig(display, attribList, &config, 1, &numConfigs))
				{
					Log::out << "Unable to make eglChooseConfig" << Log::newline;
					return ;
				}

				// Create a surface
				surface = eglCreateWindowSurface(display, config, (EGLNativeWindowType)hWnd, NULL);
				if (surface == EGL_NO_SURFACE)
				{
					Log::out << "Unable to make eglCreateWindowSurface" << Log::newline;
					return ;
				}

				// Create a GL context
				context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
				if (context == EGL_NO_CONTEXT)
				{
					Log::out << "Unable to make eglCreateContext" << Log::newline;
					return;
				}

				makeCurrent();

				eglDisplay = display;
				eglSurface = surface;
				eglContext = context;



				Log::out << "Done creating egl window, surface " << (int)surface << Log::newline;
			}

			void Window::makeCurrent()
			{
				// Make the context current
				if (!eglMakeCurrent(eglDisplay, eglSurface, eglSurface, eglContext))
				{
					Log::out << "Unable to make current" << Log::newline;
					return;
				}
			}
			void Window::unmakeCurrent()
			{
				if (eglMakeCurrent(eglDisplay, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE) {
					Log::out << "Unable to eglUnMakeCurrent" << Log::newline;
					return;
				}
			}

			void Window::swapBuffers()
			{
				eglSwapBuffers(eglDisplay, eglSurface);
			}
            

            
		}
	}
}
