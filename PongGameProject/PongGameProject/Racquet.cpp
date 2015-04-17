#include "Racquet.h"


Racquet::Racquet(float inRangeX, float inRangeY)
{
	rangeX = inRangeX;
	rangeY = inRangeY;
	radius = 0.6f;
	x = 0;
	y = 0;
	step = 0.1;
	keyPressed[0] = false;
	keyPressed[1] = false;
	keyPressed[2] = false;
	keyPressed[3] = false;
}

Racquet::Racquet(void)
{
	rangeX = 1;
	rangeY = 1;
}


Racquet::~Racquet(void)
{
}

void Racquet::keyDown(Racquet::MOVE_DIRECTION direction) {
	keyPressed[direction] = true;
}

void Racquet::keyUp(Racquet::MOVE_DIRECTION direction) {
	keyPressed[direction] = false;
}

void Racquet::move() {
	if(keyPressed[UP]) {
		if(y<(rangeY-radius)) y+=step;
	}
	if(keyPressed[DOWN]) {
		if(y>(-rangeY+radius)) y-=step;
	}
	if(keyPressed[LEFT]) {
		if(x>(-rangeX+radius)) x-=step;
	}
	if(keyPressed[RIGHT]) {
		if(x<(rangeX-radius)) x+=step;
	}

}
