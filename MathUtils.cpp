#include "MathUtils.h"

MathUtils::MathUtils() {}

MathUtils::~MathUtils() {}

float MathUtils::GetAngle(int x1, int y1, int x2, int y2) {
	float angle = 90.0f + atan2f(y1 - y2, x1 - x2) * (180.0f / M_PI);
	return angle >= 0.0f ? angle : 360.0f + angle;
}

float MathUtils::GetMagnitude(float x, float y) {
	return sqrtf((x * x) + (y * y));
}

float MathUtils::GetMagnitude(const SDL_Point& point) {
	return sqrtf((point.x * point.x) + (point.y * point.y));
}

SDL_FPoint MathUtils::GetNormalized(float x, float y) {
	float mag = GetMagnitude(x, y);
	
	SDL_FPoint res = {
		x / mag,
		y / mag
	};

	return res;
}

SDL_FPoint MathUtils::GetNormalized(const SDL_Point& point) {
	float mag = GetMagnitude(point.x, point.y);
	
	SDL_FPoint res = {
		(float)point.x / mag,
		(float)point.y / mag
	};

   
	#ifdef _DEBUG
	std::cout << "Vec: " << "(" << point.x << ", " << point.y << ");\n";
	std::cout << "Mag: " << mag << std::endl;
	#endif
	

	return res;
}
