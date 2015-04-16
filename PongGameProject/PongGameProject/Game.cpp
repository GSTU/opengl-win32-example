#include "Game.h"



Game::Game(void)
{
	Racquet r(GAME_WIDTH, GAME_HEIGHT);
	racquet = r;
}


Game::~Game(void)
{
}
