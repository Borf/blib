#pragma once

#include <blib/IWindow.h>


#ifdef WIN32
#include <Windows.h>
#endif


namespace blib
{
	namespace platform
	{
		namespace win32
		{
			class Window : public blib::IWindow
			{
			private:
				int mouseButtons;
				std::vector<long> clicks;
				int lastButton;
			protected:
				std::string className;
			public:
				HDC hdc;
				HWND hWnd;

				Window();
				virtual ~Window();


				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
				LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			};
		}
	}
}