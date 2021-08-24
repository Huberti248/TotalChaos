#include "MathUtils.h"

MathUtils::MathUtils() {}

MathUtils::~MathUtils() {}

SDL_FPoint MathUtils::CalculateCollision(float v1, float v2, float t1, float t2, float tContact) {
    float x = v2 * cosf(t2 - tContact) * cosf(tContact) + v1 * sinf(t1 - tContact) * cosf(tContact + M_PI / 2.0f);
    float y = v2 * cosf(t2 - tContact) * sinf(tContact) + v1 * sinf(t1 - tContact) * sinf(tContact + M_PI / 2.0f);

	SDL_FPoint res = {
        x,
        y
    };

	return res;
}

float MathUtils::GetAngle(int x1, int y1, int x2, int y2) {
	float angle = 90.0f + atan2f(y1 - y2, x1 - x2) * (180.0f / M_PI);
	return angle >= 0.0f ? angle : 360.0f + angle;
}

float MathUtils::GetDotProduct(SDL_FPoint x, SDL_FPoint y) {
    return x.x * y.x + x.y * y.y;
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

	return res;
}

void MathUtils::Normalize(SDL_FPoint* point) {
	float mag = GetMagnitude(point->x, point->y);

	point->x = point->x / mag;
	point->y = point->y / mag;
}

SDL_FPoint MathUtils::VectorSubstract(const SDL_FPoint& a, const SDL_FPoint& b) {
	SDL_FPoint res = {
		a.x - b.x,
		a.y - b.y
	};
	return res;
}

SDL_FPoint MathUtils::VectorSubstract(const SDL_Point& a, const SDL_Point& b) {
	SDL_FPoint res = {
		a.x - b.x,
		a.y - b.y
	};
	return res;
}

void MathUtils::VectorSubstract(SDL_Point* a, const SDL_Point& b) {
	a->x -= b.x;
	a->y -= b.y;
}

SDL_FPoint MathUtils::ToSDL_FPoint(const SDL_Point& point) {
	SDL_FPoint res = {
		(float)point.x,
		(float)point.y
	};
	return res;
}
