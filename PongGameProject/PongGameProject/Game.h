#pragma once
#include "Racquet.h"
#include "Ball.h"

#define GAME_WIDTH    1.6f
#define GAME_HEIGHT   1.0f
#define GAME_DEPTH   -7.0f
#define MESH_X_STEP   0.2f
#define MESH_Y_STEP   0.2f
#define MESH_Z_STEP   0.5f

class Game
{
public:
	Racquet racquet;
	Ball ball;
	Game(void);
	~Game(void);
};

