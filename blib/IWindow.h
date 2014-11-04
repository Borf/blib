#pragma once

#include <string.h>
#include <blib/util/FastDelegate.h>
#include <blib/config.h>

#include <string>
#include <list>
#include <vector>

namespace blib
{
	class KeyListener;
	class MouseListener;
	using namespace fastdelegate;


	class IWindow
	{
	protected:
		std::list<KeyListener*> keyListeners;
		std::list<MouseListener*> mouseListeners;
		
		int x, y;
		int width,height;
		bool resizable;
		bool showBorder;

		bool opened;
		bool active;
		std::string title;

	public:
		IWindow();
		~IWindow();

		virtual void setTitle(std::string title);
		virtual void setSize(int width, int height);
		virtual void setResizable(bool resizable);
		virtual void setBorder(bool border);
		virtual bool isOpened();
		virtual bool isActive();
		virtual int getWidth() { return width; }
		virtual int getHeight() { return height; }
		virtual int getX() { return x; };
		virtual int getY() { return y; };


		virtual void create(int icon, std::string title) = 0;
		virtual void swapBuffers() = 0;
		virtual void tick() = 0;
		virtual void makeCurrent() = 0;
		virtual void unmakeCurrent() = 0;
		virtual void onResize(int width, int height) {};
		void addListener(KeyListener* keyListener);
		void addListener(MouseListener* keyListener);
	};
}

