#pragma once
class Racquet
{
public:
	enum MOVE_DIRECTION {UP,DOWN,LEFT, RIGHT};
	float x;
	float y;
	float radius;
	Racquet(void);
	Racquet(float , float);
	~Racquet(void);
	void move(MOVE_DIRECTION);
private:
	float step;
	float rangeX;
	float rangeY;
};

