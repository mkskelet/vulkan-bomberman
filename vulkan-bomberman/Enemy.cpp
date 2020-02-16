#include "Enemy.h"

Enemy::Enemy() : loaded(false)
{

}

Enemy::Enemy(CharacterType characterType, float speed, float collisionDetectionRange, glm::vec3 position, glm::vec2 size, glm::vec2 pivot, Material* material) :
	Character(characterType, speed, collisionDetectionRange, position, size, pivot, material)
{
	dir = UpD;
	move = false;
	loaded = false;
}

Enemy::~Enemy()
{
}

void Enemy::Start()
{
	loaded = true;
}

void Enemy::Update()
{
	if (!loaded) return;

	// translate
	glm::vec2 movement(0, 0);

	if (dir == UpD)
		movement = glm::vec2(0.f, 1.f);
	else if (dir == DownD)
		movement = glm::vec2(0.f, -1.f);
	else if (dir == LeftD)
		movement = glm::vec2(-1.f, 0.f);
	else if (dir == RightD)
		movement = glm::vec2(1.f, 0.f);

	Translate(movement);

	if (colliding)
	{
		switch (dir)
		{
		case UpD:
			dir = RightD;
			break;
		case RightD:
			dir = DownD;
			break;
		case DownD:
			dir = LeftD;
			break;
		default:
			dir = UpD;
			break;
		}
	}

	// check for colision with explosions
	__super::Update();
}