#include "Ball.h"


Ball::Ball(void)
{
	x = 0;
	y = 0;
	z = 0;
	
	speedX = 0.02;
	speedY = 0.02;
	speedZ = -0.02;
}


Ball::~Ball(void)
{
	
}

void Ball::move() {
	x+=speedX;
	y+=speedY;
	z+=speedZ;
}

void Ball::setPosition(float nx, float ny) {
	x = nx;
	y = ny;
}