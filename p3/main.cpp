#include "Application.h"
#include <iostream>

int main(void)
{
	Application app;

	if (app.Init() == false) {
		return 0;
	}

	app.Run();

	app.ShutDown();
	return 0;
}