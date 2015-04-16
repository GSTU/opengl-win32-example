#include "Racquet.h"


Racquet::Racquet(float inRangeX, float inRangeY)
{
	rangeX = inRangeX;
	rangeY = inRangeY;
	radius = 0.3f;
	x = 0;
	y = 0;
	step = 0.2;
}

Racquet::Racquet(void)
{
	rangeX = 1;
	rangeY = 1;
}


Racquet::~Racquet(void)
{
}

void Racquet::move(MOVE_DIRECTION direction) {
	switch (direction)
	{
	case UP:
		if(y<(rangeY-radius)) y+=step;
		break;
	case DOWN:
		if(y>(-rangeY+radius)) y-=step;
		break;
	case LEFT:
		if(x>(-rangeX+radius)) x-=step;
		break;
	case RIGHT:
		if(x<(rangeX-radius)) x+=step;
		break;
	}
}
