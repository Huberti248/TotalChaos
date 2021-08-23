#pragma once
#include <SDL.h>

class MathUtils {
public:
    MathUtils();
    ~MathUtils();
    static float GetAngle(int x1, int y1, int x2, int y2);

};
