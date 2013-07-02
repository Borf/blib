#pragma once

#include <blib/Window.h>

namespace blib
{
	class Window
	{
	public:
		virtual void setSize(int width, int height) = 0;
		virtual void isResizable(bool resizable) = 0;
		virtual void create() = 0;
	};
}