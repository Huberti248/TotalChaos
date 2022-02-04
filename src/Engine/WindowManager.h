#pragma once
//Window manager that we can use
//to either gather information abt the window, or rescale stuff

#include "Logger.h"

//TODO: change this using a settings.dat file
#define DEFAULT_WINDOW_HEIGHT 720
#define DEFAULT_WINDOW_WIDTH 1280

class WindowManager {
public:

	WindowManager() {};
	~WindowManager() {};

	static void Rescale(int windowHeight, int windowWidth);

	static void SetWindowHeight(int windowHeight);

	static void SetWindowWidth(int windowWidth);

	static int GetWindowHeight();

	static int GetWindowWidth();

private:
	static int windowHeight;
	static int windowWidth;
};