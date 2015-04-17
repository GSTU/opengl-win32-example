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
		float pX = powf(racquet.x - ball.x, 2);
		float pY = powf(racquet.y - ball.y, 2);
		float racIn = powf(racquet.radius+ball.radius,2);

		if((pX+pY)<racIn) {
			ball.speedZ = -ball.speedZ;

			float partX = ((pX+pX)/racIn)*ball.defSpeed;
			if(ball.speedX<0) {
				ball.speedX = partX;
			} else {
				ball.speedX = -partX;
			}

			float partY = ((pY+pY)/racIn)*ball.defSpeed;
			if(ball.speedY<0) {
				ball.speedY = partY;
			} else {
				ball.speedY = -partY;
			}

			ball.speedZ+=ball.speedZStep;

		} else {
			isGameStarted = false;
		}
	}

	ball.move();
}
