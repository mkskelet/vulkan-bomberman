#pragma once

#include "Sprite.h"

class Portal : public Sprite
{
private:
	bool unlocked;
public:
	Portal();
	bool GetUnlocked() const { return unlocked; }					///< Returns true if portal is open.
	void SetUnlocked(bool unlocked) { this->unlocked = unlocked; }		///< Sets unlocked parameter of portal.
};
