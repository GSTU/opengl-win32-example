#include "Racquet.h"


Racquet::Racquet(void)
{
	x = 0;
	y = 0;
	step = 0.2;
}


Racquet::~Racquet(void)
{
}

void Racquet::move(MOVE_DIRECTION direction) {
	switch (direction)
	{
	case UP:
		y+=step;
		break;
	case DOWN:
		y-=step;
		break;
	case LEFT:
		x-=step;
		break;
	case RIGHT:
		x+=step;
		break;
	}
}
