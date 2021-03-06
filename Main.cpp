// makeASession.cpp: 定义控制台应用程序的入口点。
//
#include "AppCenter.h"
#include <signal.h>

CAppCenterPtr g_app = NULL;

void sigCtrl(int sig)
{
	g_app->stop();
}

int main(int argc, const char** argv)
{
	try
	{
		signal(SIGINT, sigCtrl);

		CAppCenterPtr app(new CAppCenter(9001));
		g_app = app;

		app->run();
	}
	catch (const std::exception& e)
	{
		// show message in herer e.what();
	}

    return 0;
}

