#include "Ball.h"


Ball::Ball(void)
{
	x = 0;
	y = 0;
}


Ball::~Ball(void)
{
}

void Ball::updatePosition(float nx, float ny) {
	x = nx;
	y = ny;
}