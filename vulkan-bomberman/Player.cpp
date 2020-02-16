#include "Player.h"

Player::Player() : loaded(false)
{
	keyStates = new bool[256];
	for (int i = 0; i < 256; i++)
		keyStates[i] = false;

	specialKeyStates = new bool[256];
	for (int i = 0; i < 256; i++)
		specialKeyStates[i] = false;

	bombCount = 1;
	bombRange = 3;
	remote = false;

	inPortal = false;
}

Player::Player(int playerNumber, CharacterType characterType, float speed, float collisionDetectionRange, glm::vec3 position, glm::vec2 size, glm::vec2 pivot, Material* material) :
	Character(characterType, speed, collisionDetectionRange, position, size, pivot, material)
{
	this->controls = ((playerNumber == PLAYER_ONE || playerNumber == PLAYER_TWO) ? playerNumber : PLAYER_ONE);
	dir = UpD;
	move = false;
	loaded = false;
	keyStates = new bool[256];
	for (int i = 0; i < 256; i++)
		keyStates[i] = false;

	specialKeyStates = new bool[256];
	for (int i = 0; i < 256; i++)
		specialKeyStates[i] = false;

	bombCount = 1;
	bombRange = 3;
	remote = false;

	inPortal = false;
}

Player::~Player()
{
}

void Player::Start()
{
	loaded = true;
}

void Player::Update()
{
	if (!loaded) return;

	// translate
	glm::vec2 movement(0.f, 0.f);

	if (controls == PLAYER_TWO)
	{
		if (keyStates['e'])
			movement = glm::vec2(0, 1);
		else if (keyStates['d'])
			movement = glm::vec2(0, -1);
		else if (keyStates['s'])
			movement = glm::vec2(-1, 0);
		else if (keyStates['f'])
			movement = glm::vec2(1, 0);
	}

	if (controls == PLAYER_ONE)
	{
		if (specialKeyStates[GLFW_KEY_UP])
			movement = glm::vec2(0, 1);
		else if (specialKeyStates[GLFW_KEY_DOWN])
			movement = glm::vec2(0, -1);
		else if (specialKeyStates[GLFW_KEY_LEFT])
			movement = glm::vec2(-1, 0);
		else if (specialKeyStates[GLFW_KEY_RIGHT])
			movement = glm::vec2(1, 0);
	}

	Translate(movement);

	// check for colision with enemy
	for (int i = 0; i < characters.size(); i++)
	{
		bool notColliding = GetPosition().x > characters[i]->GetPosition().x + characters[i]->GetScale().x ||
			characters[i]->GetPosition().x > GetPosition().x + GetScale().x ||
			GetPosition().y - GetScale().y > characters[i]->GetPosition().y ||
			characters[i]->GetPosition().y - characters[i]->GetScale().y > GetPosition().y;
		if (!notColliding)
		{
			// DED
			alive = false;
			break;
		}
	}

	// check for colision with explosions
	__super::Update();
}

void Player::KeyPress(unsigned char key)
{
	if (!loaded) return;

	keyStates[key] = true;
}

void Player::SpecialKeyPress(int key)
{
	if (!loaded) return;

	specialKeyStates[key] = true;
}

void Player::SpecialKeyRelease(int key)
{
	if (!loaded) return;

	specialKeyStates[key] = false;
}

void Player::KeyRelease(unsigned char key)
{
	if (!loaded) return;

	keyStates[key] = false;
}

void Player::UpdateCharacterColisions(std::vector<Enemy*> colliders)
{
	this->characters = colliders;
}

/*void Player::UpdatePowerupColisions(std::vector<Powerups> colliders) {
	powerups = colliders;
}*/

void Player::UpdatePortalColision(Portal* collider)
{
	portalCollider = collider;
}

bool Player::IsInPortal()
{
	// check for colision with portal
	bool notColliding = GetPosition().x > portalCollider->GetPosition().x + portalCollider->GetScale().x ||
		portalCollider->GetPosition().x > GetPosition().x + GetScale().x ||
		GetPosition().y - GetScale().y > portalCollider->GetPosition().y ||
		portalCollider->GetPosition().y - portalCollider->GetScale().y > GetPosition().y;
	if (!notColliding)
	{
		return true;
	}

	return false;
}

bool Player::PickPowerup(Powerup* powerup)
{
	// check for colision with powerups
	bool notColliding = GetPosition().x > powerup->GetPosition().x + powerup->GetScale().x ||
		powerup->GetPosition().x > GetPosition().x + GetScale().x ||
		GetPosition().y - GetScale().y > powerup->GetPosition().y ||
		powerup->GetPosition().y - powerup->GetScale().y > GetPosition().y;
	if (!notColliding)
	{
		// pick powerup
		switch (powerup->GetPowerupType())
		{
		case BombP:
			bombCount++;
			break;
		case FlameP:
			bombRange++;
			break;
		case SpeedP:
			SetSpeed(GetSpeed() * 1.1f);
			break;
		case RemoteP:
			remote = true;
			break;
		}
		return true;
	}
	return false;
}