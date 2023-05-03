#include <cmath>

#include "main/common.h"

double Utils::distance(const Position &a, const Position &b)
{
	int dx = a.x - b.x;
	int dy = a.y - b.y;
	double d_sq = (dx*dx) + (dy*dy);
	double d = sqrt(d_sq);
	return d;
}
