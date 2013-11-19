#include "Window.h"


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

	void IWindow::addListener( KeyListener* keyListener )
	{
		keyListeners.push_back(keyListener);
	}
	void IWindow::addListener( MouseListener* mouseListener )
	{
		mouseListeners.push_back(mouseListener);
	}

	IWindow::~IWindow()
	{
		Log::out<<"Window Destroyed"<<Log::newline;
	}


}

