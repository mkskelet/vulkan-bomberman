#include "Level.h"

#include <cmath>
#include <algorithm>
#include <string>
#include <cstring>
#include <cstdlib>
#include <sstream>
#include "PlayerPrefs.h"
#include "TextureDatabase.h"

// define blocks
#define LEVEL_BLOCK_BACKGROUND 11
#define LEVEL_BLOCK_SOLID 12
#define LEVEL_BLOCK_EXPLODABLE 13

#define LAYER_GAME_BACKGROUND 20.f
#define LAYER_GAME_ITEMS 15.f
#define LAYER_GAME_TOP 10.f
#define LAYER_UI_BOTTOM 5.f

std::string Level::levelName = "";

Level::Level() : Scene(SCENE_LEVEL), loaded(false), playerWon(false)
{
	timeRemaining = 1000000;
	portal = new Portal();
	p1 = nullptr;
	p2 = nullptr;
}

void Level::LoadLevel()
{
	MapLoader ml;
	map = ml.LoadMap(levelName);
}

void Level::Update()
{
	// update time
	timeRemaining = levelTime - GameTime::Instance->GetTime();

	// delete explosion if necessary
	if (explosions.size() > 0)
	{
		for (int i = explosions.size() - 1; i >= 0; i--)
		{
			if (explosions[i]->GetDisappear())
				explosions.erase(explosions.begin() + i);
		}
	}

	// update enemies
	if (ai.size() > 0)
	{
		for (int i = ai.size() - 1; i >= 0; i--)
		{
			ai[i]->UpdateGeneralColisions(solidBlocks);
			ai[i]->UpdateExplosionColisions(explosions);
			ai[i]->Update();

			if (!ai[i]->IsAlive())
				ai.erase(ai.begin() + i);
		}
	}

	// update portal
	if (ai.size() == 0)
		portal->SetUnlocked(true);

	p2->UpdateGeneralColisions(solidBlocks);
	p2->UpdateCharacterColisions(ai);
	p2->UpdateExplosionColisions(explosions);
	p2->UpdatePortalColision(portal);
	p2->Update();

	if (p1 != nullptr)
	{
		p1->UpdateGeneralColisions(solidBlocks);
		p1->UpdateCharacterColisions(ai);
		p1->UpdateExplosionColisions(explosions);
		p1->UpdatePortalColision(portal);
		p1->Update();
	}

	// check portal colision
	if (map.GetSinglePlayer() && p2->IsInPortal() && portal->GetUnlocked())
	{
		// saev score
		if (!playerWon && map.GetSinglePlayer())
			PlayerPrefs::SetInt(map.GetName(), static_cast<int>(timeRemaining));
		playerWon = true;
	}
	if (!map.GetSinglePlayer() && map.GetSinglePlayer() && p1->IsInPortal() && portal->GetUnlocked())
	{
		playerWon = true;
	}

	// check powerups colision
	if (powerups.size() > 0)
	{
		for (int i = powerups.size() - 1; i >= 0; i--)
		{
			if (p2->PickPowerup(powerups[i]))
			{
				powerups.erase(powerups.begin() + i);
			}
		}
		if (!map.GetSinglePlayer())
		{
			for (int i = powerups.size() - 1; i >= 0; i--)
			{
				if (p1->PickPowerup(powerups[i]))
				{
					powerups.erase(powerups.begin() + i);
				}
			}
		}
	}

	HandleBombExplosions(bombs1);
	HandleBombExplosions(bombs2);	

	// render endgame
	if ((map.GetSinglePlayer() && !p2->IsAlive()) || timeRemaining <= 0)
	{
		// fail screen
		endGame.SetVisible(true);
	}
	else if (map.GetSinglePlayer() && p2->IsAlive() && playerWon)
	{
		endGameP1.SetVisible(true);
	}
	else if (!map.GetSinglePlayer())
	{
		if (!p1->IsAlive() && !p2->IsAlive())
			endGameD.SetVisible(true);
		else if (!p2->IsAlive())
			endGameP1.SetVisible(true);
		else if (!p1->IsAlive())
			endGameP2.SetVisible(true);
		else if (timeRemaining <= 0)
			endGameD.SetVisible(true);
	}
}

void Level::HandleBombExplosions(std::vector<Bomb*>& bombs)
{
	// explode bombs
	if (bombs.size() > 0)
	{
		for (unsigned int i = 0; i < bombs.size(); i++)
		{
			if (bombs[i]->GetFire())
			{
				// find closest position on the grid
				glm::vec3 closest(-1.f, -1.f, LAYER_GAME_TOP);
				if (backgroundBlocks.size() > 0)
				{
					for (unsigned int k = 0; k < backgroundBlocks.size(); k++)
					{
						float c, j;
						c = sqrt(pow(fabs(bombs[i]->GetPosition().x - closest.x), 2) + pow(fabs(bombs[i]->GetPosition().y - closest.y), 2));
						j = sqrt(pow(fabs(bombs[i]->GetPosition().x - backgroundBlocks[k]->GetPosition().x), 2) + pow(fabs(bombs[i]->GetPosition().y - backgroundBlocks[k]->GetPosition().y), 2));

						if (j < c) closest = backgroundBlocks[k]->GetPosition();
					}
				}

				std::vector<int> indexToDestroy;
				bool skip = false;

				// EXPLODE RIGHT SIDE
				for (int j = 0; j < bombs[i]->GetRange(); j++)
				{
					glm::vec3 next(closest.x + j * backgroundBlocks[0]->GetScale().x, closest.y, LAYER_GAME_TOP);

					// create explosion effect
					if (!skip)
					{
						Explosion* b = new Explosion();
						b->SetPosition(next);
						b->SetScale(p2->GetScale());
						explosions.push_back(b);
					}

					if (solidBlocks.size() > 0 && !skip)
					{
						for (unsigned int k = 0; k < solidBlocks.size(); k++)
						{
							if (solidBlocks[k]->GetPosition().x <= next.x + 2 && solidBlocks[k]->GetPosition().x >= next.x - 2
								&& solidBlocks[k]->GetPosition().y <= next.y + 2 && solidBlocks[k]->GetPosition().y >= next.y - 2)
							{
								if (IsBlockExplodable(solidBlocks[i]))
								{
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if (IsBlockSolid(solidBlocks[i]))
								{
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;

				// EXPLODE LEFT SIDE
				for (int j = 1; j < bombs[i]->GetRange(); j++)
				{
					glm::vec3 next(closest.x - j * backgroundBlocks[0]->GetScale().x, closest.y, LAYER_GAME_TOP);

					// create explosion effect
					if (!skip)
					{
						Explosion* b = new Explosion();
						b->SetPosition(next);
						b->SetScale(p2->GetScale());
						explosions.push_back(b);
					}

					if (solidBlocks.size() > 0 && !skip)
					{
						for (unsigned int k = 0; k < solidBlocks.size(); k++)
						{
							if (solidBlocks[k]->GetPosition().x <= next.x + 2 && solidBlocks[k]->GetPosition().x >= next.x - 2
								&& solidBlocks[k]->GetPosition().y <= next.y + 2 && solidBlocks[k]->GetPosition().y >= next.y - 2)
							{
								if (IsBlockExplodable(solidBlocks[i]))
								{
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if (IsBlockSolid(solidBlocks[i]))
								{
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;

				// EXPLODE TOP SIDE
				for (int j = 1; j < bombs[i]->GetRange(); j++)
				{
					glm::vec3 next(closest.x, closest.y + j * backgroundBlocks[0]->GetScale().y, LAYER_GAME_TOP);

					// create explosion effect
					if (!skip)
					{
						Explosion* b = new Explosion();
						b->SetPosition(next);
						b->SetScale(p2->GetScale());
						explosions.push_back(b);
					}

					if (solidBlocks.size() > 0 && !skip)
					{
						for (unsigned int k = 0; k < solidBlocks.size(); k++)
						{
							if (solidBlocks[k]->GetPosition().x <= next.x + 2 && solidBlocks[k]->GetPosition().x >= next.x - 2
								&& solidBlocks[k]->GetPosition().y <= next.y + 2 && solidBlocks[k]->GetPosition().y >= next.y - 2)
							{
								if (IsBlockExplodable(solidBlocks[i]))
								{
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if (IsBlockSolid(solidBlocks[i]))
								{
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}
				skip = false;

				// EXPLODE BOTTOM SIDE
				for (int j = 1; j < bombs[i]->GetRange(); j++)
				{
					glm::vec3 next(closest.x, closest.y - j * backgroundBlocks[0]->GetScale().y, LAYER_GAME_TOP);

					// create explosion effect
					if (!skip)
					{
						Explosion* b = new Explosion();
						b->SetPosition(next);
						b->SetScale(p2->GetScale());
						explosions.push_back(b);
					}

					if (solidBlocks.size() > 0 && !skip)
					{
						for (unsigned int k = 0; k < solidBlocks.size(); k++)
						{
							if (solidBlocks[k]->GetPosition().x <= next.x + 2 && solidBlocks[k]->GetPosition().x >= next.x - 2
								&& solidBlocks[k]->GetPosition().y <= next.y + 2 && solidBlocks[k]->GetPosition().y >= next.y - 2)
							{
								if (IsBlockExplodable(solidBlocks[i]))
								{
									indexToDestroy.push_back(k);
									skip = true;
									break;
								}
								else if (IsBlockSolid(solidBlocks[i]))
								{
									explosions.erase(explosions.begin() + explosions.size() - 1);
									skip = true;
									break;
								}
							}
						}
					}
				}

				std::sort(indexToDestroy.begin(), indexToDestroy.end(), std::greater<int>());
				for (unsigned int j = 0; j < indexToDestroy.size(); j++)
				{
					// vymazanim posuvam indexy, treba sortnut vektor a mazat od najvacsieho indexu
					// najprv vyrenderujeme exploziu

					// potom vymazeme objekty
					solidBlocks.erase(solidBlocks.begin() + indexToDestroy[j]);
				}
				indexToDestroy.clear();

				// explode				
				bombs.erase(bombs.begin() + i);
			}
		}
	}
}

void Level::Start()
{
	srand(time(NULL));

	LoadLevel();
	levelTime = map.GetTimeLimit() * 1000 + GameTime::Instance->GetTime();

	GenerateLevel();

	// calculate max index for powerups
	int maxIndex = 0;
	if (solidBlocks.size() > 0)
	{
		for (unsigned int i = 0; i < solidBlocks.size(); i++)
		{
			if (IsBlockExplodable(solidBlocks[i]))
				maxIndex++;
		}
	}

	// generate portal
	if (map.GetSinglePlayer())
	{
		int portalIndex = rand() % maxIndex;
		takenNodes.push_back(portalIndex);

		int tempIndex = 0;
		if (solidBlocks.size() > 0)
		{
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == portalIndex)
					{
						// set portal position
						portal->SetPosition(solidBlocks[i]->GetPosition());
						portal->SetScale(solidBlocks[i]->GetScale());
						break;
					}

					tempIndex++;
				}
			}
		}
	}

	// generate bomb powerups
	for (int i = 0; i < map.GetBombPowerups(); i++)
	{
		bool free = false;
		do
		{
			int powerupIndex = rand() % maxIndex;

			if (find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;

			takenNodes.push_back(powerupIndex);

			int tempIndex = 0;
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == powerupIndex)
					{
						// set powerup position
						Powerup* p = new Powerup();
						p->SetPowerupType(BombP);
						p->SetPosition(solidBlocks[i]->GetPosition());
						p->SetScale(solidBlocks[i]->GetScale());

						powerups.push_back(p);
						break;
					}

					tempIndex++;
				}
			}

			free = true;
		} while (free != true);
	}

	// generate flame powerups
	for (int i = 0; i < map.GetFlamePowerups(); i++)
	{
		bool free = false;
		do
		{
			int powerupIndex = rand() % maxIndex;

			if (find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;

			takenNodes.push_back(powerupIndex);

			int tempIndex = 0;
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == powerupIndex)
					{
						// set powerup position
						Powerup* p = new Powerup();
						p->SetPowerupType(FlameP);
						p->SetPosition(solidBlocks[i]->GetPosition());
						p->SetScale(solidBlocks[i]->GetScale());

						powerups.push_back(p);
						break;
					}

					tempIndex++;
				}
			}

			free = true;
		} while (free != true);
	}

	// generate bomb powerups
	for (int i = 0; i < map.GetBombPowerups(); i++)
	{
		bool freee = false;
		do
		{
			int powerupIndex = rand() % maxIndex;

			if (find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;

			takenNodes.push_back(powerupIndex);

			int tempIndex = 0;
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == powerupIndex)
					{
						// set powerup position
						Powerup* p = new Powerup();
						p->SetPowerupType(BombP);
						p->SetPosition(solidBlocks[i]->GetPosition());
						p->SetScale(solidBlocks[i]->GetScale());

						powerups.push_back(p);
						break;
					}

					tempIndex++;
				}
			}

			freee = true;
		} while (freee != true);
	}

	// generate speed powerups
	for (int i = 0; i < map.GetSpeedPowerups(); i++)
	{
		bool freee = false;
		do
		{
			int powerupIndex = rand() % maxIndex;

			if (find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;

			takenNodes.push_back(powerupIndex);

			int tempIndex = 0;
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == powerupIndex)
					{
						// set powerup position
						Powerup* p = new Powerup();
						p->SetPowerupType(SpeedP);
						p->SetPosition(solidBlocks[i]->GetPosition());
						p->SetScale(solidBlocks[i]->GetScale());

						powerups.push_back(p);
						break;
					}

					tempIndex++;
				}
			}

			freee = true;
		} while (freee != true);
	}

	// generate remote powerups
	for (int i = 0; i < map.GetRemotePowerups(); i++)
	{
		bool freee = false;
		do
		{
			int powerupIndex = rand() % maxIndex;

			if (find(takenNodes.begin(), takenNodes.end(), powerupIndex) != takenNodes.end())
				continue;

			takenNodes.push_back(powerupIndex);

			int tempIndex = 0;
			for (unsigned int i = 0; i < solidBlocks.size(); i++)
			{
				if (IsBlockExplodable(solidBlocks[i]))
				{
					if (tempIndex == powerupIndex)
					{
						// set powerup position
						Powerup* p = new Powerup();
						p->SetPowerupType(RemoteP);
						p->SetPosition(solidBlocks[i]->GetPosition());
						p->SetScale(solidBlocks[i]->GetScale());

						powerups.push_back(p);
						break;
					}

					tempIndex++;
				}
			}

			freee = true;
		} while (freee != true);
	}
}

bool Level::IsBlockExplodable(Sprite* block)
{
	if (strcmp(block->GetMaterial()->GetTexture()->GetPath(), "../sprites/blocks/ExplodableBlock.tga"))
	{
		return true;
	}
	return false;
}

bool Level::IsBlockSolid(Sprite* block)
{
	if (strcmp(block->GetMaterial()->GetTexture()->GetPath(), "../sprites/blocks/SolidBlock.tga"))
	{
		return true;
	}
	return false;
}

void Level::GenerateLevel()
{
	float w = 1.0f;
	float h = 1.0f;

	glm::vec2 topLeftPivot(0.f, 1.f);
	Shader* shader = Shader::Find("uber");
	Texture* texture;
	Material* material;

	topBar = Sprite(glm::vec3(0, h, LAYER_UI_BOTTOM), glm::vec2(w, 0.05f), topLeftPivot);
	topBar.SetColor(0.2f, 0.2f, 0.2f);
	texture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/screen.tga");
	material = new Material(shader, texture);
	topBar.SetMaterial(material);
	Sprite::AddToMap(&topBar);

	endGame = Sprite(glm::vec3(0, h / 3 * 2, LAYER_UI_BOTTOM), glm::vec2(w, h / 3), topLeftPivot);
	texture = TextureDatabase::GetInstance().GetTexture("../sprites/GG.tga");
	material = new Material(shader, texture);
	endGame.SetMaterial(material);
	endGame.SetVisible(false);
	Sprite::AddToMap(&endGame);

	endGameP1 = Sprite(glm::vec3(0, h / 3 * 2, LAYER_UI_BOTTOM), glm::vec2(w, h / 3), topLeftPivot);
	texture = TextureDatabase::GetInstance().GetTexture("../sprites/P1.tga");
	material = new Material(shader, texture);
	endGameP1.SetMaterial(material);
	endGameP1.SetVisible(false);
	Sprite::AddToMap(&endGameP1);

	endGameP2 = Sprite(glm::vec3(0, h / 3 * 2, LAYER_UI_BOTTOM), glm::vec2(w, h / 3), topLeftPivot);
	texture = TextureDatabase::GetInstance().GetTexture("../sprites/P2.tga");
	material = new Material(shader, texture);
	endGameP2.SetMaterial(material);
	endGameP2.SetVisible(false);
	Sprite::AddToMap(&endGameP2);

	endGameD = Sprite(glm::vec3(0, h / 3 * 2, LAYER_UI_BOTTOM), glm::vec2(w, h / 3), topLeftPivot);
	texture = TextureDatabase::GetInstance().GetTexture("../sprites/DRAW.tga");
	material = new Material(shader, texture);
	endGameD.SetMaterial(material);
	endGameD.SetVisible(false);
	Sprite::AddToMap(&endGameD);

	// clear map
	backgroundBlocks.clear();
	solidBlocks.clear();
	//otherBlocks.clear();

	glm::vec2 boundaries(w, h - 25);

	float xCount = map.GetX();
	float yCount = map.GetY();
	float xOffset = 0;
	float yOffset = 0;

	// calculate block size and offsets
	float blockSize = boundaries.y / yCount;
	if (blockSize * xCount <= boundaries.x)
	{
		xOffset = (boundaries.x - (blockSize * xCount)) / 2;
	}
	else
	{
		blockSize = boundaries.x / xCount;
		yOffset = (boundaries.y - (blockSize * yCount)) / 2;
	}

	int c = 0;
	for (int i = 0; i < map.GetY(); i++)
	{
		for (int j = 0; j < map.GetX(); j++)
		{
			texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/BackgroundTile.tga");
			material = new Material(shader, texture);

			bool drawStatic = false;
			bool drawNothing = false;
			// switch to determine texture, block type accessed by map[c++]
			int b = map[c++];
			switch (b)
			{
			case BLOCK_SOLID:
				texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/SolidBlock.tga");
				material = new Material(shader, texture);
				drawStatic = true;
				break;
			case BLOCK_EXPLODABLE:
				texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/ExplodableBlock.tga");
				material = new Material(shader, texture);
				drawStatic = true;
				break;
			case BLOCK_PLAYER:
				if (loaded)
				{
					if (!map.GetSinglePlayer())
					{
						p1 = new Player(PLAYER_ONE, PlayerCharacter, blockSize / 15, 5, glm::vec3(xOffset + j * blockSize + blockSize / 8, boundaries.y - i * blockSize - yOffset - blockSize / 8, LAYER_GAME_TOP),
							glm::vec2(blockSize * 6 / 8, blockSize * 6 / 8), topLeftPivot, material);
						p1->Start();
					}
					break;
				}
				p2 = new Player(PLAYER_TWO, PlayerCharacter, blockSize / 15, 5, glm::vec3(xOffset + j * blockSize + blockSize / 8, boundaries.y - i * blockSize - yOffset - blockSize / 8, LAYER_GAME_TOP),
					glm::vec2(blockSize * 6 / 8, blockSize * 6 / 8), topLeftPivot, material);
				p2->Start();
				drawNothing = true;
				loaded = true;
				break;
			default:
				drawNothing = true;
				if (b >= 0 && b <= 9)
				{
					Enemy* e = new Enemy(EnemyCharacter, blockSize / 15, 5, glm::vec3(xOffset + j * blockSize + blockSize / 8, boundaries.y - i * blockSize - yOffset - blockSize / 8, LAYER_GAME_TOP),
						glm::vec2(blockSize * 6 / 8, blockSize * 6 / 8), topLeftPivot, material);
					e->Start();

					ai.push_back(e);
				}
				break;
			}

			// draw background
			Sprite* bkg = new Sprite(
				glm::vec3(xOffset + j * blockSize, boundaries.y - i * blockSize - yOffset, LAYER_GAME_BACKGROUND),
				glm::vec2(blockSize, blockSize),
				topLeftPivot
			);
			bkg->SetMaterial(material);
			Sprite::AddToMap(bkg);
			backgroundBlocks.push_back(bkg);

			if (drawStatic && !drawNothing)
			{
				Sprite* solid = new Sprite(
					glm::vec3(xOffset + j * blockSize, boundaries.y - i * blockSize - yOffset, LAYER_GAME_TOP),
					glm::vec2(blockSize, blockSize),
					topLeftPivot
				);
				solid->SetMaterial(material);
				Sprite::AddToMap(solid);
				solidBlocks.push_back(solid);
			}

			/*if (!drawStatic)
			{
				otherBlocks.push_back(
					Sprite(
						glm::vec3(xOffset + j * blockSize, boundaries.y - i * blockSize - yOffset, LAYER_GAME_BACKGROUND),
						glm::vec2(blockSize, blockSize),
						topLeftPivot
					)
				);
			}*/
		}
	}
}

void Level::KeyPress(int key)
{
	// escape to exit to main menu
	if (key == GLFW_KEY_ESCAPE)
	{
		SwitchScene(SCENE_MAIN_MENU);
	}

	p2->KeyPress(key);

}

void Level::SpecialKeyPress(int key)
{
	
}

void Level::KeyRelease(int key)
{
	p2->KeyRelease(key);

	// p2 bomb plant
	if (key == GLFW_KEY_A && (unsigned int)p2->GetBombCount() > bombs1.size() && p2->IsAlive())
	{
		PlantBomb(p2, bombs1);
	}
	if (key == GLFW_KEY_Q && bombs1.size() > 0 && p2->IsAlive())
	{
		bombs1[0]->SetFire(true);
	}

	// p1 bomb plant
	if (map.GetSinglePlayer())
		return;
	if (key == GLFW_KEY_L && (unsigned int)p1->GetBombCount() > bombs2.size() && p1->IsAlive())
	{
		PlantBomb(p1, bombs2);
	}
	if (key == GLFW_KEY_K && bombs2.size() > 0 && p1->IsAlive())
	{
		bombs2[0]->SetFire(true);
	}
}

void Level::PlantBomb(Player* player, std::vector<Bomb*>& bombs)
{
	Bomb* b = new Bomb();
	b->SetPosition(player->GetPosition());
	b->SetScale(player->GetScale());
	b->SetRemoteFireEnabled(player->GetRemoteFireEnabled());
	b->SetRange(player->GetBombRange());

	glm::vec2 closest(-1, -1);
	if (backgroundBlocks.size() > 0)
	{
		for (unsigned int i = 0; i < backgroundBlocks.size(); i++)
		{
			float c, j;
			c = sqrt(pow(fabs(b->GetPosition().x - closest.x), 2) + pow(fabs(b->GetPosition().y - closest.y), 2));
			j = sqrt(pow(fabs(b->GetPosition().x - backgroundBlocks[i]->GetPosition().x), 2) + pow(fabs(b->GetPosition().y - backgroundBlocks[i]->GetPosition().y), 2));

			if (j < c) closest = backgroundBlocks[i]->GetPosition();
		}
	}
	b->SetPosition(glm::vec3(closest.x + player->GetScale().x / 6, closest.y - player->GetScale().y / 6, LAYER_GAME_TOP));
	bombs.push_back(b);
}

void Level::SpecialKeyRelease(int key)
{
	
}