#include "MathUtils.h"

MathUtils::MathUtils() {}

MathUtils::~MathUtils() {}

float MathUtils::GetAngle(int x1, int y1, int x2, int y2) {
    float angle = 90.0f + atan2f(y1 - y2, x1 - x2) * (180.0f / M_PI);
    return angle >= 0.0f ? angle : 360.0f + angle;
}
