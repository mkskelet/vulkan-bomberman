#pragma once

#include "Sprite.h"
#include "GameTime.h"

#define TIME_TO_DISAPPEAR 1000

class Explosion : public Sprite
{
private:
	float timeToDisappear;
public:
	Explosion();
	bool GetDisappear() const;
};