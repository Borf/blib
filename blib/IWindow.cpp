#include "Window.h"


#include <blib/linq.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/util/Log.h>
using blib::util::Log;
#include <map>



namespace blib
{

	IWindow::IWindow()
	{
		opened = false;
		active = false;
	}







	void IWindow::setSize( int width, int height )
	{
		this->width = width;
		this->height = height;
	}

	void IWindow::setResizable( bool resizable )
	{
		this->resizable = resizable;
	}

	void IWindow::setTitle(std::string title)
	{
		this->title = title;
	}

	void IWindow::setBorder(bool border)
	{
		this->showBorder = border;
	}
	
	bool IWindow::isOpened()
	{
		return opened;
	}

	void IWindow::addListener( KeyListener* keyListener)
	{
		keyListeners.push_back(keyListener);
	}
	void IWindow::addListener( MouseListener* mouseListener, bool highPrio)
	{
		if(highPrio)
			mouseListeners.emplace_front(mouseListener);
		else
			mouseListeners.push_back(mouseListener);
	}

	IWindow::~IWindow()
	{
//		blib::linq::deleteall(mouseListeners);
//		blib::linq::deleteall(keyListeners);
		Log::out<<"Window Destroyed"<<Log::newline;
	}

	bool IWindow::isActive()
	{
		return active;
	}

	void IWindow::moveTo(int x, int y)
	{
		this->x = x;
		this->y = y;
	}


}

