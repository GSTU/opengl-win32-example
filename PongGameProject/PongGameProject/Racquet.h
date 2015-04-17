#pragma once
class Racquet
{
public:
	enum MOVE_DIRECTION {UP =0 ,DOWN=1,LEFT=2, RIGHT=3};
	float x;
	float y;
	float radius;
	Racquet(void);
	Racquet(float , float);
	~Racquet(void);
	void move();
	void keyDown(MOVE_DIRECTION);
	void keyUp(MOVE_DIRECTION);
private:
	float step;
	float rangeX;
	float rangeY;
	bool keyPressed[4];
};

