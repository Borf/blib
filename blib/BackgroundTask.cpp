#include "BackgroundTask.h"
#include "App.h"

namespace blib
{

	BackgroundTask::BackgroundTask( App* app, std::function<void()> toRun, std::function<void()> whenDone ) : util::Thread("BackgroundWorker")
	{
		this->app = app;
		this->toRun = toRun;
		this->whenDone = whenDone;
		start();
	}


	int BackgroundTask::run()
	{
		toRun();
		if(app)
			app->runLater(whenDone);
		return 0;
	}

}