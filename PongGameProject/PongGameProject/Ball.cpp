#include "Ball.h"


Ball::Ball(void)
{
	x = 0;
	y = 0;
	z = 0;
	
	defSpeed = 0.04;
	speedX = -0.01;
	speedY = -0.02;
	speedZ = -defSpeed;
	speedZStep = -0.004;

	radius = 0.2;
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