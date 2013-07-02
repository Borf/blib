#include "Window.h"

#include <blib/KeyListener.h>
#include <blib/util/Log.h>
using blib::util::Log;
#include <map>

namespace blib
{
	LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
		if(message == WM_NCCREATE)
			SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) ((CREATESTRUCT*)lParam)->lpCreateParams);
		return ((Window*) GetWindowLongPtr(hWnd, GWL_USERDATA))->wndProc(hWnd, message, wParam, lParam);
	}

	Window::Window()
	{
		opened = false;
	}

	void Window::create()
	{
		//Open Window
		const char* title = "WindowTitle";

		WNDCLASS windowClass;
		DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;

		HINSTANCE hInstance = GetModuleHandle(NULL);

		windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
		windowClass.lpfnWndProc = (WNDPROC) WndProc;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = hInstance;
		windowClass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
		windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		windowClass.hbrBackground = NULL;
		windowClass.lpszMenuName = NULL;
		windowClass.lpszClassName = title;

		if (!RegisterClass(&windowClass)) {
			return;
		}
		hWnd = CreateWindowEx(dwExStyle, title, title, (showBorder ? WS_OVERLAPPEDWINDOW : (WS_OVERLAPPED | WS_POPUP)),
			CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, this);
		hdc = GetDC(hWnd); // Get the device context for our window
		opened = true;
	}


	LRESULT Window::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message) {
		case WM_SIZE: // If our window is resizing
			width = LOWORD(lParam);
			height = HIWORD(lParam);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		case WM_KEYDOWN:
			for(std::list<KeyListener*>::iterator it = keyListeners.begin(); it != keyListeners.end(); it++)
				(*it)->onKeyDown((blib::Key)wParam);
			break;
		case WM_KEYUP:
			for(std::list<KeyListener*>::iterator it = keyListeners.begin(); it != keyListeners.end(); it++)
				(*it)->onKeyUp((blib::Key)wParam);
			break;
	/*	case WM_LBUTTONDOWN:
			if(mouseDriver)
				mouseDriver->mouseDown(MouseButtonDeviceDriver::Left);
			break;
		case WM_LBUTTONUP:
			if(mouseDriver)
				mouseDriver->mouseUp(MouseButtonDeviceDriver::Left);
			break;
		case WM_RBUTTONDOWN:
			if(mouseDriver)
				mouseDriver->mouseDown(MouseButtonDeviceDriver::Right);
			break;
		case WM_RBUTTONUP:
			if(mouseDriver)
				mouseDriver->mouseUp(MouseButtonDeviceDriver::Right);
			break;
		case WM_MBUTTONDOWN:
			middleButtonDown = true;
			lastX = GET_X_LPARAM(lParam); 
			lastY = GET_Y_LPARAM(lParam); 
			break;
		case WM_MBUTTONUP:
			middleButtonDown = false;
			break;
		case WM_MOUSEMOVE:
			if(middleButtonDown && simPositionDriver)
			{
				int x = GET_X_LPARAM(lParam); 
				int y = GET_Y_LPARAM(lParam); 
				simPositionDriver->mouseMove(x-lastX, y-lastY);
				lastX = x;
				lastY = y;
			}
			break;*/

		}

		return DefWindowProc(hWnd, message, wParam, lParam);
	}




	void Window::setSize( int width, int height )
	{
		this->width = width;
		this->height = height;
	}

	void Window::setResizable( bool resizable )
	{
		this->resizable = resizable;
	}

	void Window::setBorder(bool border)
	{
		this->showBorder = border;
	}
	
	bool Window::isOpened()
	{
		return opened;
	}

	void Window::addListener( KeyListener* keyListener )
	{
		keyListeners.push_back(keyListener);
	}
	void Window::addListener( MouseListener* mouseListener )
	{
		mouseListeners.push_back(mouseListener);
	}


}