#pragma once

#include "framework.h"

class cMath
{
public:
	float GetAngleTwoPoints(ImVec2 p1, ImVec2 p2);
	float DistanceOfPointToPoint(ImVec2 p1, ImVec2 p2);
	float DistanceOfPointToLine(ImVec2 p1, ImVec2 p2, ImVec2 p);
};
extern cMath* Math;