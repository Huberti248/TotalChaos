#pragma once
#include <SDL.h>
#include <math.h>

#ifdef _DEBUG
#include <iostream>
#endif

class MathUtils {
public:
    MathUtils();
    ~MathUtils();
    
    static float GetAngle(int x1, int y1, int x2, int y2);
    
    static float GetMagnitude(float x, float y);

    static float GetMagnitude(const SDL_Point &point);

    static SDL_FPoint GetNormalized(float x, float y);

    static SDL_FPoint GetNormalized(const SDL_Point &point); //Maybe make a version with SDL_FPoint parameters

    static void Normalize(SDL_FPoint* point);

    static SDL_FPoint VectorSubstract(const SDL_FPoint& a, const SDL_FPoint& b);

    static SDL_FPoint VectorSubstract(const SDL_Point& a, const SDL_Point& b);

    static void VectorSubstract(SDL_Point* a, const SDL_Point& b);

    static SDL_FPoint ToSDL_FPoint(const SDL_Point& point);
};
