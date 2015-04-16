#pragma once
#include "Racquet.h"
#include "Ball.h"

class Game
{
public:
	Racquet racquet;
	Ball ball;
	Game(void);
	~Game(void);
};

