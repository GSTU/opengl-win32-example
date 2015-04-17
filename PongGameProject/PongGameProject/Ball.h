#pragma once
class Ball
{
public:
	float x;
	float y;
	float z;
	float radius;
	float speedX;
	float speedY;
	float speedZ;
	float defSpeed;
	float speedZStep;

	Ball(void);
	~Ball(void);
	void setPosition(float , float);
	void move();
};

