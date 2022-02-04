#include "Math.h"
float cMath::get_angle_2points(int p1x, int p1y, int p2x, int p2y)
{
	int deltaY = p2y - p1y;
	int deltaX = p2x - p1x;
	float angleInDegrees = atan2(deltaY, deltaX);
	return angleInDegrees;
}