#pragma once
#include "Racquet.h"
#include "Ball.h"
#include "PlayField.h"
#include <cmath>

class Game
{
public:
	Racquet racquet;
	Ball ball;
	PlayField playField;
	Game(void);
	~Game(void);
	void moveBall();
	bool isGameStarted;
};

