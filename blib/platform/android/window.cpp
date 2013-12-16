#include <blib/platform/android/window.h>

#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>


#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/util/Log.h>

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
			Window::Window()
			{
			}
			Window::~Window()
			{
			}


			void Window::create(int icon, std::string title)
			{
				surface = 0;
			    const EGLint attribs[] = {
			            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
			            EGL_BLUE_SIZE, 8,
			            EGL_GREEN_SIZE, 8,
			            EGL_RED_SIZE, 8,
			            EGL_OPENGL_ES2_BIT,
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

			    surface = eglCreateWindowSurface(display, config, androidApp->window, NULL);
			    context = eglCreateContext(display, config, NULL, NULL);

			    eglQuerySurface(display, surface, EGL_WIDTH, &w);
				eglQuerySurface(display, surface, EGL_HEIGHT, &h);

				Log::out<<"Surface of "<<w<<", "<<h<<Log::newline;
				if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
						Log::out<<"Unable to eglMakeCurrent"<<Log::newline;
						return;
				}
				Log::out<<"Done creating egl window, surface "<<(int)surface<<Log::newline;
			}

			void Window::swapBuffers()
			{
			    Log::out<<"eglSwapBuffers "<<(int)surface<<Log::newline;
				eglSwapBuffers(display, surface);
			}


			void Window::makeCurrent()
			{
				if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
					Log::out<<"Unable to eglMakeCurrent"<<Log::newline;
						return;
				}
			}
			void Window::unmakeCurrent()
			{
				if (eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE) {
					Log::out<<"Unable to eglUnMakeCurrent"<<Log::newline;
						return;
				}
			}


		}
	}
}
