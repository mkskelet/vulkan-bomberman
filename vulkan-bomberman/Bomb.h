#pragma once

#include "Sprite.h"
#include "GameTime.h"

#define TIME_TO_EXPLODE 3000

class Bomb : public Sprite
{
private:
	int range;				///< Range of bomb explosion.
	float explosionTime;			///< Time when bomb explodes.
	bool remoteFireEnabled;		///< Is remote fire enabled?
	bool fire;					///< Signal to explode.
public:
	Bomb();
	void SetRange(int range) { this->range = range; }						///< Set bomb range.
	void SetRemoteFireEnabled(bool enabled) { remoteFireEnabled = enabled; }		///< Enable / disable remote.
	void SetFire(bool fire) { this->fire = fire; }								///< Set fire. Pass true to explode a bomb if remote is enabled.
	bool GetFire() const;												///< Get fire. Returns true if bomb should explode.
	int GetRange() const { return range; }									///< 	Returns range of bomb.
	bool GetRemoteFireEnabled() const { return remoteFireEnabled; }			///< Returns true if remote fire is enabled.	
};