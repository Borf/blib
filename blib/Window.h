#pragma once

#include <blib/config.h>

#if defined(BLIB_WIN)
#include <blib/platform/win32/window.h>
#elif defined(BLIB_LINUX)
#include <blib/platform/linux/window.h>
#else

#endif



namespace blib
{
#if defined(BLIB_WIN)
	class Window : public platform::win32::Window
#elif defined(BLIB_LINUX)
	class Window : public platform::linux::Window
#else
	class Window : public IWindow
#endif
	{
	};

}