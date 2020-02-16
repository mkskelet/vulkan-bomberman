#include "Character.h"
#include "Say.h"

Character::Character() : characterType(PlayerCharacter), speed(5.0f), collisionDetectionRange(5), colliding(false), alive(true)
{

}

Character::Character(CharacterType characterType, float speed, float collisionDetectionRange)
{
	this->characterType = characterType;
	this->speed = speed;
	this->collisionDetectionRange = collisionDetectionRange;
	colliding = false;
	alive = true;
}

Character::Character(CharacterType characterType, float speed, float collisionDetectionRange, glm::vec3 position, glm::vec2 size, glm::vec2 pivot, Material* material) :
	Sprite(position, size, pivot)
{
	this->SetMaterial(material);
	Sprite::AddToMap(this);
	this->characterType = characterType;
	this->speed = speed;
	this->collisionDetectionRange = collisionDetectionRange;
	colliding = false;
	alive = true;
}

void Character::Translate(glm::vec2 translation)
{
	glm::vec2 movement = translation * speed;
	glm::vec3 newPos = GetPosition() + glm::vec3(movement.x, movement.y, 0.f);

	for (int i = 0; i < colliders.size(); i++)
	{
		bool notColliding = newPos.x > colliders[i]->GetPosition().x + colliders[i]->GetScale().x ||
			colliders[i]->GetPosition().x > newPos.x + GetScale().x ||
			newPos.y - GetScale().y > colliders[i]->GetPosition().y ||
			colliders[i]->GetPosition().y - colliders[i]->GetScale().y > newPos.y;
		if (!notColliding)
		{
			newPos = GetPosition();
			colliding = true;
			break;
		}
		else colliding = false;
	}

	Say::Log("- Character move", GetPosition().x, GetPosition().y, "->", newPos.x, newPos.y);
	SetPosition(newPos);
}

void Character::UpdateGeneralColisions(std::vector<Sprite*> colliders)
{
	this->colliders = colliders;
}

void Character::UpdateExplosionColisions(std::vector<Explosion*> colliders)
{
	exColliders = colliders;
}

void Character::Start()
{
}

void Character::Update()
{
	for (int i = 0; i < exColliders.size(); i++)
	{
		bool notColliding = GetPosition().x > exColliders[i]->GetPosition().x + exColliders[i]->GetScale().x ||
			exColliders[i]->GetPosition().x > GetPosition().x + GetScale().x ||
			GetPosition().y - GetScale().y > exColliders[i]->GetPosition().y ||
			exColliders[i]->GetPosition().y - exColliders[i]->GetScale().y > GetPosition().y;
		if (!notColliding)
		{
			alive = false;
		}
	}
}