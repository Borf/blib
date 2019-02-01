#include <blib/platform/nx/window.h>

#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>
#include <blib/util/Log.h>
#include <blib/Util.h>
#include <blib/App.h>
#include <blib/gl/GlResizeRegister.h>

#include <GL/glew.h>
#include <nn/nn_Assert.h>
#include <nv/nv_MemoryManagement.h>
#include <nn/hid.h>
#include <nn/nn_Log.h>


using blib::util::Log;

namespace blib
{
	namespace platform
	{
		namespace nx
		{
			void* NvAllocateFunction(size_t size, size_t alignment, void* userPtr)
			{
				NN_UNUSED(userPtr);
				// According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
				return aligned_alloc(alignment, nn::util::align_up(size, alignment));
			}
			void NvFreeFunction(void* addr, void* userPtr)
			{
				NN_UNUSED(userPtr);
				free(addr);
			}
			void* NvReallocateFunction(void* addr, size_t newSize, void* userPtr)
			{
				NN_UNUSED(userPtr);
				return realloc(addr, newSize);
			}

			void* NvDevtoolsAllocateFunction(size_t size, size_t alignment, void* userPtr)
			{
				NN_UNUSED(userPtr);
				// According to specifications of aligned_alloc(), we need to coordinate the size parameter to become the integral multiple of alignment.
				return aligned_alloc(alignment, nn::util::align_up(size, alignment));
			}
			void NvDevtoolsFreeFunction(void* addr, void* userPtr)
			{
				NN_UNUSED(userPtr);
				free(addr);
			}
			void* NvDevtoolsReallocateFunction(void* addr, size_t newSize, void* userPtr)
			{
				NN_UNUSED(userPtr);
				return realloc(addr, newSize);
			}


			Window::Window(App* app)
			{
				this->app = app;
			}
			Window::~Window()
			{
			}


			void Window::create(int icon, std::string title)
			{
				nn::hid::InitializeTouchScreen();
				/*
				* Set memory allocator for graphics subsystem.
				* This function must be called before using any graphics API's.
				*/
				nv::SetGraphicsAllocator(NvAllocateFunction, NvFreeFunction, NvReallocateFunction, NULL);

				/*
				* Set memory allocator for graphics developer tools and NVN debug layer.
				* This function must be called before using any graphics developer features.
				*/
				nv::SetGraphicsDevtoolsAllocator(NvDevtoolsAllocateFunction, NvDevtoolsFreeFunction, NvDevtoolsReallocateFunction, NULL);
				/*
				* Donate memory for graphics driver to work in.
				* This function must be called before using any graphics API's.
				*/
				size_t graphicsSystemMemorySize = 8 * 1024 * 1024;
				void* graphicsHeap = malloc(graphicsSystemMemorySize);
				nv::InitializeGraphics(graphicsHeap, graphicsSystemMemorySize);

				/*
				* Initialize Video Interface (VI) system to display
				* to the target's screen
				*/
				nn::vi::Initialize();

				nn::Result result = nn::vi::OpenDefaultDisplay(&s_pDisplay);
				NN_ASSERT(result.IsSuccess());

				result = nn::vi::CreateLayer(&s_pLayer, s_pDisplay);
				NN_ASSERT(result.IsSuccess());

				result = nn::vi::SetLayerScalingMode(s_pLayer, nn::vi::ScalingMode_FitToLayer);
				NN_ASSERT(result.IsSuccess());

				result = nn::vi::GetNativeWindow(&s_NativeWindowHandle, s_pLayer);
				NN_ASSERT(result.IsSuccess());

				/*
				* Initialize EGL
				*/
				EGLBoolean eglResult;

				s_Display = ::eglGetDisplay(EGL_DEFAULT_DISPLAY);
				NN_ASSERT(s_Display != NULL, "eglGetDisplay failed.");

				eglResult = ::eglInitialize(s_Display, 0, 0);
				NN_ASSERT(eglResult, "eglInitialize failed.");

				EGLint configAttribs[] = {
					/*
					* Require OpenGL ES2/ES3 support.
					* If you need OpenGL (not ES) supprot, you should specify EGL_OPENGL_BIT.
					*/
#ifdef USE_OPENGL_ES
					EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
#else
					EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
#endif
					EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
					EGL_RED_SIZE, 8,
					EGL_GREEN_SIZE, 8,
					EGL_BLUE_SIZE, 8,
					EGL_ALPHA_SIZE, 8,
					EGL_STENCIL_SIZE, 8,
					EGL_NONE
				};
				EGLint numConfigs = 0;
				EGLConfig config;
				eglResult = ::eglChooseConfig(s_Display, configAttribs, &config, 1, &numConfigs);
				NN_ASSERT(eglResult && numConfigs == 1, "eglChooseConfig failed.");

				s_Surface = ::eglCreateWindowSurface(s_Display, config, static_cast<NativeWindowType>(s_NativeWindowHandle), 0);
				NN_ASSERT(s_Surface != EGL_NO_SURFACE, "eglCreateWindowSurface failed.");

				/*
				* Set the current rendering API.
				*/
#ifdef USE_OPENGL_ES
				eglResult = eglBindAPI(EGL_OPENGL_ES_API);
#else
				eglResult = eglBindAPI(EGL_OPENGL_API);
#endif
				NN_ASSERT(eglResult, "eglBindAPI failed.");

				/*
				* Create new context and set it as current.
				*/
				EGLint contextAttribs[] = {
					/* Set target garaphics api version. */
#ifdef USE_OPENGL_ES
					EGL_CONTEXT_MAJOR_VERSION, 3,
					EGL_CONTEXT_MINOR_VERSION, 2,
#else
					EGL_CONTEXT_MAJOR_VERSION, 4,
					EGL_CONTEXT_MINOR_VERSION, 5,
#endif
					/* For debug callback */
					EGL_CONTEXT_FLAGS_KHR, EGL_CONTEXT_OPENGL_DEBUG_BIT_KHR,
					EGL_NONE
				};
				s_Context = ::eglCreateContext(s_Display, config, EGL_NO_CONTEXT, contextAttribs);
				NN_ASSERT(s_Context != EGL_NO_CONTEXT, "eglCreateContext failed. %d", eglGetError());

				eglResult = ::eglMakeCurrent(s_Display, s_Surface, s_Surface, s_Context);
				NN_ASSERT(eglResult, "eglMakeCurrent failed.");

				::glewInit();
				Log::out<<"Done creating egl window"<<Log::newline;

				glGenVertexArrays(1, &vao);
				glBindVertexArray(vao);


			}

			bool touching = false;
			int lastX, lastY;
			void Window::tick()
			{

				nn::hid::TouchScreenState<nn::hid::TouchStateCountMax> state = {};
				nn::hid::GetTouchScreenState(&state);
//				nn::hid::TouchScreenState<nn::hid::TouchStateCountMax> states[nn::hid::TouchScreenStateCountMax] = {};

				// At least one input state can be obtained.
//				int stateCount = nn::hid::GetTouchScreenStates(states, sizeof(states) / sizeof(states[0]));
				bool found = false;
				for (int j = 0; j < state.count; ++j)
				{
					lastX = state.touches[j].x;
					lastY = state.touches[j].y;
					if(!touching)
						touchDownEvent(state.touches[j].fingerId, state.touches[j].x, state.touches[j].y);
					touchMoveEvent(state.touches[j].fingerId, state.touches[j].x, state.touches[j].y);
					touching = true;
					found = true;
					break;
				}
				if (touching && !found)
				{
					touchUpEvent(0, lastX, lastY);
					touching = false;
				}
				glBindVertexArray(vao);

			}


			void Window::swapBuffers()
			{
				::eglSwapBuffers(s_Display, s_Surface);
			}


			bool Window::makeCurrent()
			{
				if (eglMakeCurrent(s_Display, s_Surface, s_Surface, s_Context) == EGL_FALSE) {
					Log::out<<"Unable to eglMakeCurrent"<<Log::newline;
						return false;
				}
				return true;
			}
			void Window::unmakeCurrent()
			{
			/*	if (eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT) == EGL_FALSE) {
					Log::out<<"Unable to eglUnMakeCurrent"<<Log::newline;
						return;
				}*/
			}

			void Window::touchDownEvent(unsigned long id, int x, int y)
			{
				Log::out << "Touch down " << id << " at " << x << ", " << y << Log::newline;
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

			void Window::touchUpEvent(unsigned long id, int x, int y)
			{
				x = x + blib::gl::GlResizeRegister::offx;
				Log::out << "Touch up " << id << " at " << x << ", " << y << Log::newline;
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

			void Window::touchMoveEvent(unsigned long id, int x, int y)
			{
				x = x + blib::gl::GlResizeRegister::offx;
				Log::out << "Touch move " << id << " at " << x << ", " << y << Log::newline;
				for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseMove(x,y,MouseListener::Left);

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

			void Window::keyDownEvent(blib::Key key)
			{
				x = x + blib::gl::GlResizeRegister::offx;
				for (std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
					(*it)->onMouseMove(x, y, MouseListener::Left);

			/*	for (Touch& t : app->touches)
				{
					if (t.id == id)
					{
						t.position.x = x;
						t.position.y = y;
						break;
					}
				}*/
			}



		}
	}
}
