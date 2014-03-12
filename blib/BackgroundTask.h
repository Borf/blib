#pragma once

#include <functional>
#include <blib/util/Thread.h>
#include <blib/App.h>

namespace blib
{
	class App;

	template<class T>
	class BackgroundTask : public util::Thread
	{
		App* app;
		std::function<T()> toRun;
		std::function<void(T)> whenDone;

	public:
		BackgroundTask(App* app, std::function<T()> toRun, std::function<void(T)> whenDone = [](T param){}) : blib::util::Thread("BackgroundWorker")
		{
			if (!app || app->appSetup.threaded || app->appSetup.backgroundTasks)
			{
				this->app = app;
				this->toRun = toRun;
				this->whenDone = whenDone;
				start();
			}
			else
			{
				whenDone(toRun());
			}
		}

		virtual int run()
		{
			T returnValue = toRun();
			if(app)
				app->runLater(whenDone, returnValue);
			return 0;
		}


	};

}
