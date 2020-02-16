#pragma once

#include "Character.h"
#include "Enemy.h"
#include "Portal.h"
#include "Powerup.h"
#include <vector>

// players constants
#define PLAYER_ONE 1
#define PLAYER_TWO 2

class Player : public Character
{
private:
	bool loaded;						///< Signals if player was already loaded.
	/*bool* keyStates;					///< Keyboard signals.
	bool* specialKeyStates;				///< Special keys.*/
	bool move;
	int controls;						///< Determines which controls to use (PLAYER_ONE or PLAYER_TWO)
	Direction dir;						///< Direction of movement
	std::vector<Enemy*> characters;		///< Characters to test colision against.
	Portal* portalCollider;					///< Player gets notofied of portal's position so it can detect colision.	
	bool inPortal;						///< True if player is on the portal.
	int bombCount;					///< Maximum number of bombs that player can plant at once.	
	int bombRange;					///< Maximum range of bomb explosion.
	bool remote;						///< Signals if remote fire is enabled.
	
	std::map<int, bool> keyMap;
	bool IsKeyPressed(int key);

public:
	Player();
	~Player();
	Player(int playerNumber, CharacterType characterType, float speed, float collisionDetectionRange, glm::vec3 position, glm::vec2 size, glm::vec2 pivot, Material* material);
	void Start();
	void Update();
	void KeyPress(int key);
	void KeyRelease(int key);
	void UpdateCharacterColisions(std::vector<Enemy*> colliders);			///< Updates the character colision flags.
	void UpdatePortalColision(Portal* collider);								///< Updates portal collider.
	bool IsInPortal();												///< Check collision with portal.
	bool PickPowerup(Powerup* powerup);								///< Returns true if player is standing on a powerup.
	int GetBombCount() const { return bombCount; }					///< Returns amount of bombs that the player can plant at once.
	int GetBombRange() const { return bombRange; }					///< Returns bomb range.
	bool GetRemoteFireEnabled() { return remote; }						///< Returns true if remote fire is enabled.
};