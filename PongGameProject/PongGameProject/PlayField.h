#pragma once
 
class PlayField
{
public:
	PlayField(void);
	~PlayField(void);
	float width;
	float height;
	float depth;

	float meshXStep;
	float meshYStep;
	float meshZStep;
};

