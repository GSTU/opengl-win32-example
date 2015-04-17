#include "Game.h"



Game::Game(void)
{
	Racquet r(playField.width, playField.height);
	racquet = r;
}


Game::~Game(void)
{
}

void Game::moveBall() {
	
	if(ball.x< -playField.width || ball.x>playField.width) {
		ball.speedX = -ball.speedX;
	}

	if(ball.y < -playField.height||ball.y > playField.height) {
		ball.speedY = -ball.speedY;
	}

	if(ball.z < playField.depth || ball.z > 0) {
		ball.speedZ = -ball.speedZ;
	}

	ball.move();
}
