#include "WindowManager.h"

int WindowManager::windowHeight = DEFAULT_WINDOW_HEIGHT;
int WindowManager::windowWidth = DEFAULT_WINDOW_WIDTH;

void WindowManager::Rescale(int windowHeight, int windowWidth) {
	LOG_LN("Rescale not implemented!");
}

void WindowManager::SetWindowHeight(int windowHeight) {
	LOG_LN("SetWindowHeight not implemented!");
}

void WindowManager::SetWindowWidth(int windowWidth) {
	LOG_LN("SetWindowWidth not implemented!");
}

int WindowManager::GetWindowHeight() {
	return WindowManager::windowHeight;
}

int WindowManager::GetWindowWidth() {
	return WindowManager::windowWidth;
}
