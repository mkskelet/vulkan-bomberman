#pragma once

#include "Sprite.h"

enum PowerupType { BombP = 19, FlameP = 20, SpeedP = 21, RemoteP = 22 };

class Powerup : public Sprite
{
private:
	PowerupType pt;		///< Powerup type.
public:
	Powerup();
	void SetPowerupType(PowerupType pt);
	PowerupType GetPowerupType() const { return pt; }
};