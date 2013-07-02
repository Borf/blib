#pragma once

#include <blib/Window.h>
#include <blib/util/FastDelegate.h>


#ifdef WIN32
#include <Windows.h>
#endif

#include <list>

namespace blib
{
	class KeyListener;
	class MouseListener;
	using namespace fastdelegate;

	class Window
	{
	private:
		std::list<KeyListener*> keyListeners;
		std::list<MouseListener*> mouseListeners;
	protected:
		HDC hdc;
		HWND hWnd;

		int width,height;
		bool resizable;
		bool showBorder;

		bool opened;

	public:
		Window();

		virtual void setSize(int width, int height);
		virtual void setResizable(bool resizable);
		virtual void setBorder(bool border);
		virtual bool isOpened();

		virtual void create();
		virtual void swapBuffers() = 0;
		virtual void tick() = 0;

		LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		void addListener(KeyListener* keyListener);
		void addListener(MouseListener* keyListener);

	};
}