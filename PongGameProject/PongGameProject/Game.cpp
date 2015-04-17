#include "Game.h"



Game::Game(void)
{
	isGameStarted = true;
	Racquet r(playField.width, playField.height);
	racquet = r;
	isPause = false;
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

	if(ball.z < playField.depth ) {
		ball.speedZ = -ball.speedZ;
	}

	if( ball.z > 0 ) {

		bool isGood = powf(racquet.x - ball.x, 2) + powf(racquet.y - ball.y, 2) < powf(racquet.radius+ball.radius,2);

		if(isGood) {
			ball.speedZ = -ball.speedZ;
		} else {
			isGameStarted = false;
		}
	}

	ball.move();
}
