#include <blib/platform/win32/window.h>

#ifdef WIN32
#include <Windowsx.h>
#endif

#include <blib/util/Log.h>
#include <blib/KeyListener.h>
#include <blib/MouseListener.h>
#include <blib/gl/Window.h>

using blib::util::Log;

namespace blib
{
	namespace platform
	{
		namespace win32
		{
			LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
				if(message == WM_NCCREATE)
					SetWindowLongPtr(hWnd, GWL_USERDATA, (LONG_PTR) ((CREATESTRUCT*)lParam)->lpCreateParams);
				return ((Window*) GetWindowLongPtr(hWnd, GWL_USERDATA))->wndProc(hWnd, message, wParam, lParam);
			}

			BOOL _SetConsoleIcon(HICON hIcon)
			{
				typedef BOOL (WINAPI *SetConsoleIcon_t)(HICON handle);
				SetConsoleIcon_t SetConsoleIcon;
				HMODULE hDll = GetModuleHandleA("kernel32");
				if(hDll)
				{
					SetConsoleIcon = (SetConsoleIcon_t)GetProcAddress(hDll, "SetConsoleIcon");
					if(SetConsoleIcon)
						return SetConsoleIcon(hIcon);
				}
				return false;
			}



			Window::Window()
			{
				mouseButtons = 0;
			}
			Window::~Window()
			{
				CloseWindow(hWnd);
			}


			void Window::create(int icon, std::string title)
			{
				showBorder = true;
				if(className == "")
				{
					className = "blib_";
					for(int i = 0; i < 3; i++)
						className += 'a' + (rand()%26);
				}
				WNDCLASS windowClass;
				DWORD dwExStyle = WS_EX_APPWINDOW ;//| WS_EX_WINDOWEDGE;

				HINSTANCE hInstance = GetModuleHandle(NULL);

				windowClass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
				windowClass.lpfnWndProc = (WNDPROC) WndProc;
				windowClass.cbClsExtra = 0;
				windowClass.cbWndExtra = 0;
				windowClass.hInstance = hInstance;
				windowClass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(icon));
				windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
				windowClass.hbrBackground = NULL;
				windowClass.lpszMenuName = NULL;
				windowClass.lpszClassName = "asdasdasd";

				_SetConsoleIcon(LoadIcon(hInstance, MAKEINTRESOURCE(icon))); //scary shit yo
				if (!RegisterClass(&windowClass)) {
					Log::out<<"Error loading window class"<<Log::newline;
					return;
				}
				hWnd = CreateWindowEx(dwExStyle, "asdasdasd", title.c_str(), WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS | WS_POPUP,
					CW_USEDEFAULT, CW_USEDEFAULT, width, height, NULL, NULL, hInstance, this);
				if(!hWnd)
				{
					Log::out<<"Unable to create window...wtf?"<<Log::newline;
					char* lpMsgBuf;
					DWORD dw = GetLastError(); 
					FormatMessage(
						FORMAT_MESSAGE_ALLOCATE_BUFFER | 
						FORMAT_MESSAGE_FROM_SYSTEM |
						FORMAT_MESSAGE_IGNORE_INSERTS,
						NULL,
						dw,
						MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
						(LPTSTR) &lpMsgBuf,
						0, NULL );

					Log::out<<"Error: "<<lpMsgBuf<<Log::newline;
					return;

				}
				hdc = GetDC(hWnd); // Get the device context for our window

				ShowWindow(hWnd, SW_SHOW);
			}


			LRESULT Window::wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
			{
				size_t clickCount = 0;
				if(message == WM_LBUTTONDOWN || message == WM_MBUTTONDOWN || message == WM_RBUTTONDOWN)
				{
					clickCount = 1;
					if(message != lastButton)
						clicks.clear();
					lastButton = message;
					clicks.push_back(GetTickCount());
					int i = clicks.size()-2;
					while(i >= 0 && clicks[i] > clicks[i+1]-200)
					{
						i--;
						clickCount++;
					}
					if(clickCount < clicks.size())
						clicks.erase(clicks.begin(), clicks.begin() + clicks.size() - clickCount);
				}

				if(message == WM_LBUTTONUP || message == WM_MBUTTONUP || message == WM_RBUTTONUP)
					clickCount = clicks.size();
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
				case WM_CHAR:
					for(std::list<KeyListener*>::iterator it = keyListeners.begin(); it != keyListeners.end(); it++)
						(*it)->onChar((char)wParam);
					break;
				case WM_LBUTTONDOWN:
					mouseButtons |= MouseListener::Left;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseDown(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Left, clickCount);
					break;
				case WM_LBUTTONUP:
					mouseButtons &= ~MouseListener::Left;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseUp(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Left, clickCount);
					break;
				case WM_RBUTTONDOWN:
					mouseButtons |= MouseListener::Right;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseDown(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Right, clickCount);
					break;
				case WM_RBUTTONUP:
					mouseButtons &= ~MouseListener::Right;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseUp(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Right, clickCount);
					break;
				case WM_MBUTTONDOWN:
					mouseButtons |= MouseListener::Middle;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseDown(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Middle, clickCount);
					break;
				case WM_MBUTTONUP:
					mouseButtons &= ~MouseListener::Middle;
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseUp(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),MouseListener::Middle, clickCount);
					break;
				case WM_MOUSEMOVE:
					for(std::list<MouseListener*>::iterator it = mouseListeners.begin(); it != mouseListeners.end(); it++)
						(*it)->onMouseMove(GET_X_LPARAM(lParam),GET_Y_LPARAM(lParam),(MouseListener::Buttons)mouseButtons);
					break;
				}

				return DefWindowProc(hWnd, message, wParam, lParam);
			}

			void Window::makeCurrent()
			{
				if(!wglMakeCurrent(hdc, ((blib::gl::Window*)this)->hrc))
				{
					Log::out<<"ERROR MAKING CURRENT"<<Log::newline;
					char* lpMsgBuf;
					FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM |FORMAT_MESSAGE_IGNORE_INSERTS, NULL,	GetLastError(),	MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),(LPTSTR) &lpMsgBuf,0, NULL );
					Log::out<<"Error: "<<lpMsgBuf<<Log::newline;
				}
			}
			void Window::unmakeCurrent()
			{
				wglMakeCurrent(NULL, NULL);
			}


		}
	}
}
