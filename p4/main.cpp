#include "Application.h"
#include "Singleton.h"

#include <iostream>

int main(void)
{
	Application & app = Application::Get();

	if (app.Init() == false) {
		return 0;
	}

	app.Run();

	app.ShutDown();
	return 0;
}