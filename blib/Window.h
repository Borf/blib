#pragma once

#include <blib/Window.h>
#include <string.h>
#include <blib/util/FastDelegate.h>


#ifdef WIN32
#include <Windows.h>
#endif

#include <string>
#include <list>
#include <vector>

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

		int mouseButtons;
		std::vector<long> clicks;
		int lastButton;

	protected:

		int width,height;
		bool resizable;
		bool showBorder;

		bool opened;
		std::string title;
		std::string className;
#ifdef WIN32
		HWND hWnd;
	public:
		HDC hdc;
#else

	public:
#endif





		Window();
		virtual ~Window();

		virtual void setTitle(std::string title);
		virtual void setSize(int width, int height);
		virtual void setResizable(bool resizable);
		virtual void setBorder(bool border);
		virtual bool isOpened();
		virtual int getWidth() { return width; }
		virtual int getHeight() { return height; }

		virtual void create(int icon, std::string title);
		virtual void swapBuffers() = 0;
		virtual void tick() = 0;

#ifdef WIN32
		LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
#endif

		void addListener(KeyListener* keyListener);
		void addListener(MouseListener* keyListener);
	};
}
