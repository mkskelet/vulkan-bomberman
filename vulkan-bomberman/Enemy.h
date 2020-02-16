#pragma once

#include "Character.h"

class Enemy : public Character
{
private:
	bool loaded;
	bool move;
	int controls;
	Direction dir;
public:
	Enemy();
	~Enemy();
	Enemy(CharacterType characterType, float speed, float collisionDetectionRange, glm::vec3 position, glm::vec2 size, glm::vec2 pivot, Material* material);
	void Start();
	void Update();
};