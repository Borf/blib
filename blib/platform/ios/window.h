#pragma once

#include <blib/IWindow.h>

#include <blib/util/Log.h>
using blib::util::Log;


namespace blib
{
	class App;
	namespace platform
	{
		namespace ios
		{
			class Window : public blib::IWindow
			{
			private:
				App* app;
			public:
				Window(App* app);
				virtual ~Window();

				std::vector<long> clicks;


				virtual void swapBuffers();
				virtual void create(int icon, std::string title);
				virtual void makeCurrent();
				virtual void unmakeCurrent();
                virtual void tick() { }
                
                void setWidth(int w) { width = w; };
                void setHeight(int h) { height = h; };
                
                
                void touchDownEvent(int x, int y);
                void touchUpEvent(int x, int y);
                void touchMoveEvent(int x, int y);
			};
		}
	}
}
