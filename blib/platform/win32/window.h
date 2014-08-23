#pragma once

#include <blib/IWindow.h>


#include <Windows.h>


namespace blib
{
	class App;
	namespace platform
	{
		namespace win32
		{
			class Window : public blib::IWindow
			{
			private:
				App* app;
				int mouseButtons;
				std::vector<long> clicks;
				int lastButton;
			protected:
				std::string className;
			public:
				HDC hdc;
				HWND hWnd;

				Window(App* app);
				virtual ~Window();

				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
				LRESULT wndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

			};
		}
	}
}
