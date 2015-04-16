#pragma once
class Racquet
{
public:
	enum MOVE_DIRECTION {UP,DOWN,LEFT, RIGHT};
	float x;
	float y;
	Racquet(void);
	~Racquet(void);
	void move(MOVE_DIRECTION);
private:
	float step;
};

