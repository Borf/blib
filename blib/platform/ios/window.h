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
				virtual bool makeCurrent();
				virtual void unmakeCurrent();
                virtual void tick() { }
                
                void setWidth(int w) { width = w; };
                void setHeight(int h) { height = h; };
                
                
                void touchDownEvent(unsigned long id, int x, int y);
                void touchUpEvent(unsigned long id, int x, int y);
                void touchMoveEvent(unsigned long id, int x, int y);
			};
		}
	}
}
