#include "Math.h"

cMath* Math = new cMath();

float cMath::GetAngleTwoPoints(ImVec2 p1, ImVec2 p2)
{
	int deltaY = p2.y - p1.y;
	int deltaX = p2.x - p1.x;
	float angleInDegrees = atan2(deltaY, deltaX);
	return angleInDegrees;
}

float cMath::DistanceOfPointToPoint(ImVec2 p1, ImVec2 p2) {
	return (float)sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}

float cMath::DistanceOfPointToLine(ImVec2 p1, ImVec2 p2, ImVec2 p)
{
	//          (y1-y2)x + (x2-x1)y + (x1y2-x2y1)
	//d(P,L) = --------------------------------
	//         sqrt( (x2-x1)pow2 + (y2-y1)pow2 )

	float ch = (p1.y - p2.y) * p.x + (p2.x - p1.x) * p.y + (p1.x * p2.y - p2.x * p1.y);
	float del = sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));
	float d = ch / del;
	return d;
}