#pragma once

#include <blib/config.h>
#include <blib/platform/win32/window.h>
namespace blib
{
#ifdef BLIB_WIN
	class Window : public platform::win32::Window
#endif
	{
	};

}