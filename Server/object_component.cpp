#pragma once

#include "common.cpp"

class objectComponent {

public:

	float32 positionX = rand() % 1000;		//previously 500.f (for middle of screen)
	float32 positionY = rand() % 1000;		//previously 500.f (for middle of screen)
	uint16 rotation = 0;
	float32 velocityX = 0.f;
	float32 velocityY = 0.f;


};