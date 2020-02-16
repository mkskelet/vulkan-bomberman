#include "Explosion.h"

Explosion::Explosion() : Sprite(glm::vec3(0.f, 0.f, 0.f), glm::vec2(0.f, 0.f), glm::vec2(0.0f, 1.0f))
{
	timeToDisappear = GameTime::Instance->GetTime() + TIME_TO_DISAPPEAR;
}

bool Explosion::GetDisappear() const
{
	if (timeToDisappear <= GameTime::Instance->GetTime())
		return true;

	else return false;
}