#pragma once

#include <blib/config.h>

#if defined(BLIB_WIN)
#include <blib/platform/win32/window.h>
#elif defined(BLIB_LINUX)
#include <blib/platform/linux/window.h>
#elif defined(BLIB_ANDROID)
#include <blib/platform/android/window.h>
#elif defined(BLIB_IOS)
#include <blib/platform/ios/window.h>
#elif defined(BLIB_EMSCRIPTEN)
#include <blib/platform/emscripten/window.h>
#elif defined(BLIB_NX)
#include <blib/platform/nx/window.h>
#else
#include <blib/IWindow.h>
#endif



namespace blib
{
	class App;

#if defined(BLIB_WIN)
	#define WINDOW_SUPER platform::win32::Window
#elif defined(BLIB_LINUX)
	#define WINDOW_SUPER platform::linux::Window
#elif defined(BLIB_ANDROID)
	#define WINDOW_SUPER platform::android::Window
#elif defined(BLIB_IOS)
    #define WINDOW_SUPER platform::ios::Window
#elif defined(BLIB_EMSCRIPTEN)
	#define WINDOW_SUPER platform::emscripten::Window
#elif defined(BLIB_NX)
	#define WINDOW_SUPER platform::nx::Window
#else
	class Window : public IWindow
	#define WINDOW_SUPER IWindow
#endif
	class Window : public WINDOW_SUPER
	{
	public:
		Window(App* app);
	};

}
