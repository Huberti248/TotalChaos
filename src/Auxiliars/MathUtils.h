#pragma once
#include <SDL.h>
#include <math.h>
#include <iostream>

class MathUtils {
public:

    static const SDL_FPoint WORLD_UP;
    static const SDL_FPoint WORLD_DOWN;
    static const SDL_FPoint WORLD_LEFT;
    static const SDL_FPoint WORLD_RIGHT;

    MathUtils();
    ~MathUtils();
    
    static SDL_FPoint CalculateCollision(float v1, float v2, float t1, float t2, float tContact);

    static float GetAngle(int x1, int y1, int x2, int y2);

    static float GetDotProduct(SDL_FPoint x, SDL_FPoint y);
    
    static float GetMagnitude(float x, float y);

    static float GetMagnitude(const SDL_Point &point);

    static float GetMagnitudeSqr(const SDL_FPoint &point);

    static SDL_FPoint GetNormalized(float x, float y);

    static SDL_FPoint GetNormalized(const SDL_Point &point);

    static SDL_FPoint GetNormalized(const SDL_FPoint &point);

    static void Normalize(SDL_FPoint* point);

    static SDL_FPoint VectorSubstract(const SDL_FPoint& a, const SDL_FPoint& b);

    static SDL_FPoint VectorSubstract(const SDL_Point& a, const SDL_Point& b);

    static void VectorSubstract(SDL_Point* a, const SDL_Point& b);

    static SDL_FPoint VectorAdd(const SDL_FPoint& a, const SDL_FPoint& b);

    static float DistanceSqr(const SDL_FPoint& a, const SDL_FPoint& b);

    static SDL_FPoint ToSDL_FPoint(const SDL_Point& point);

    static float Clamp(float n, float lower, float upper);

    static int Random(int min, int max);

    static int RoundToClosestInteger(float n);

    static bool Between(int value, int lower, int upper);

};
