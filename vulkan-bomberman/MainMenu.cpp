#include "MainMenu.h"
#include "TextureDatabase.h"
#include <iostream>
#include "Say.h"

//block texture IDs
#define PREVIEW_BLOCK_BACKGROUND 0
#define PREVIEW_BLOCK_SOLID 1
#define PREVIEW_BLOCK_EXPLODABLE 2
#define PREVIEW_BLOCK_BOMBERMAN 3
#define PREVIEW_BLOCK_CREEP 4
#define PREVIEW_BLOCK_BOMB 5
#define PREVIEW_BLOCK_EXPLOSION 6
#define PREVIEW_BLOCK_PORTAL 7

/// Constructor for main menu scene, sets player count and ai count to lowest needed for the game to start.
MainMenu::MainMenu() : Scene(SCENE_MAIN_MENU),
showMainMenu(true),
previewMapIndex(0),
singlePlayer(true),
showControls(false),
menuPosition(SINGLE_PLAYER),
mapName("")
{
	Shader* shader = Shader::Find("uber");

	// allocate materials
	materials.resize(8);

	Texture* texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/BackgroundTile.tga");
	Material* mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_BACKGROUND] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/SolidBlock.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_SOLID] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/ExplodableBlock.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_EXPLODABLE] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Bman_F_f00.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_BOMBERMAN] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Creep_F_f00.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_CREEP] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Bomb.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_BOMB] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Flame.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_EXPLOSION] = mat;

	texture = TextureDatabase::GetInstance().GetTexture("../sprites/blocks/Portal.tga");
	mat = new Material(shader, texture);
	materials[PREVIEW_BLOCK_PORTAL] = mat;
}

/// Function to render scene.
void MainMenu::Render()
{

}

/// Method called when scene is created. 
void MainMenu::Start()
{
	showControls = true;
	/*if (PlayerPrefs::GetBool("First run", true))
	{
		showControls = true;
		PlayerPrefs::SetBool("First run", false);
	}*/
	OnWindowResized();
}

/// Function to update scene.
void MainMenu::Update()
{
	if (showMainMenu)
	{
		// draw 1 player button
		if (menuPosition == SINGLE_PLAYER)
			spButton.SetColor(0.8f, 0, 0);
		else spButton.SetColor(0.8f, 0.8f, 0);

		// draw 2 players button
		if (menuPosition == MULTI_PLAYER)
			mpButton.SetColor(0.8f, 0, 0);
		else mpButton.SetColor(0.8f, 0.8f, 0);

		// draw controls button
		if (menuPosition == MENU_CONTROLS)
			controlsButton.SetColor(0.8f, 0, 0);
		else controlsButton.SetColor(0.8f, 0.8f, 0);

			// draw exit button
		if (menuPosition == MENU_EXIT)
			exitButton.SetColor(0.8f, 0, 0);
		else exitButton.SetColor(0.8f, 0.8f, 0);
	}
	else
	{
		//Say::Log("no show main menu");

		// print map name
		float w = 1.0f;
		float h = 1.0f;

		/*glColor3f(0.7f, 0.0f, 1.0f);
		glRasterPos2f(w / 2 - 90, h / 4 * 3 + 25);
		{
			const char* arr;
			if (mapLoader.GetMapList(singlePlayer).size() > previewMapIndex)
				arr = mapLoader.GetMapList(singlePlayer)[previewMapIndex].c_str();
			else arr = (char*)"NO MAP FOUND.";
			int len = (int)strlen(arr);
			for (int i = 0; i < len; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr[i]);
			}
		}
		glColor3f(1.0f, 1.0f, 1.0f);*/

		// print high score
		/*stringstream ss;
		glColor3f(0.7f, 0.0f, 1.0f);
		glRasterPos2f(w / 2 - 90, h / 4 * 3 + 5);
		if (mapLoader.GetMapList(singlePlayer).size() > previewMapIndex&& PlayerPrefs::GetInt(mapLoader.GetMapList(singlePlayer)[previewMapIndex], -1) != -1)
		{
			ss << PlayerPrefs::GetInt(mapLoader.GetMapList(singlePlayer)[previewMapIndex], -1);
			string score = ss.str();
			const char* arr1 = score.c_str();
			int len = (int)strlen(arr1);
			for (int i = 0; i < len; i++)
			{
				glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr1[i]);
			}
		}
		glColor3f(1.0f, 1.0f, 1.0f);*/

		// print map
		/*if (mapPreview.size() > 0)
		{
			for (int i = 0; i < mapPreview.size(); i++)
				mapPreview[i].Render();
		}*/

		// print navigation arrows
		if (previewMapIndex == 0)
		{
			//arrowPrev.SetTexture(TextureDatabase::GetInstance().GetTexture("../sprites/menu/ls_menu.tga"));
		}
		else
		{
			//arrowPrev.SetTexture(TextureDatabase::GetInstance().GetTexture("../sprites/menu/ls_back.tga"));
		}

		if (mapLoader.GetMapList(singlePlayer).size() > (previewMapIndex + 1))
		{
			//arrowNext.Render();
		}
	}
}

/// Method called whenever window has been resized
void MainMenu::OnWindowResized()
{
	float w = 1.0f;
	float h = 1.0f;
	float menuTop = 0.33f;
	float buttonHeight = 0.1f;
	float buttonWidth = 0.4f;

	// main menu
	Shader* shader = Shader::Find("uber");
	Texture* loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/screen.tga");
	Material* material = new Material(shader, loadedTexture);
	background = Sprite(glm::vec3(0.5f, 0.5f, 20.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.5f));
	background.SetMaterial(material);
	Sprite::AddToMap(&background);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/mm_1pl.tga");
	material = new Material(shader, loadedTexture);
	spButton = Sprite(glm::vec3(0.05f, menuTop, 10.0f), glm::vec2(buttonWidth, buttonHeight), glm::vec2(0.0f, 1.0f));
	spButton.SetMaterial(material);
	Sprite::AddToMap(&spButton);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/mm_2pl.tga");
	material = new Material(shader, loadedTexture);
	menuTop += buttonHeight;
	mpButton = Sprite(glm::vec3(0.05f, menuTop, 10.0f), glm::vec2(buttonWidth, buttonHeight), glm::vec2(0.0f, 1.0f));
	mpButton.SetMaterial(material);
	Sprite::AddToMap(&mpButton);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/mm_ctrl.tga");
	material = new Material(shader, loadedTexture);
	menuTop += buttonHeight;
	controlsButton = Sprite(glm::vec3(0.05f, menuTop, 10.0f), glm::vec2(buttonWidth, buttonHeight), glm::vec2(0.0f, 1.0f));
	controlsButton.SetMaterial(material);
	Sprite::AddToMap(&controlsButton);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/mm_exit.tga");
	material = new Material(shader, loadedTexture);
	menuTop += buttonHeight;
	exitButton = Sprite(glm::vec3(0.05f, menuTop, 10.0f), glm::vec2(buttonWidth, buttonHeight), glm::vec2(0.0f, 1.0f));
	exitButton.SetMaterial(material);
	Sprite::AddToMap(&exitButton);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/mm_ctrlbox.tga");
	material = new Material(shader, loadedTexture);
	menuTop -= 4 * buttonHeight;
	controlsBox = Sprite(glm::vec3(w - (h / 3 * 2), h * 5 / 6, 10.0f), glm::vec2(h / 3 * 2, h / 3 * 2), glm::vec2(0.0f, 1.0f));
	controlsBox.SetColor(0, 0, 0.8f);
	controlsBox.SetMaterial(material);
	Sprite::AddToMap(&controlsBox);

	// preview
	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/ls_back.tga");
	material = new Material(shader, loadedTexture);
	arrowPrev = Sprite(glm::vec3(10, h / 2 + buttonHeight / 2, 10.0f), glm::vec2(buttonWidth / 2, buttonHeight / 2), glm::vec2(0.0f, 1.0f));
	arrowPrev.SetMaterial(material);
	Sprite::AddToMap(&arrowPrev);

	loadedTexture = TextureDatabase::GetInstance().GetTexture("../sprites/menu/ls_next.tga");
	material = new Material(shader, loadedTexture);
	arrowNext = Sprite(glm::vec3(w - 10 - buttonWidth / 2, h / 2 + buttonHeight / 2, 10.0f), glm::vec2(buttonWidth / 2, buttonHeight / 2), glm::vec2(0.0f, 1.0f));
	arrowNext.SetMaterial(material);
	Sprite::AddToMap(&arrowNext);

	//LoadMapPreview();
}

/// Method called when key is pressed
void MainMenu::KeyPress(int key)
{
	if (key == GLFW_KEY_ENTER)
	{
		// go to level select menu
		switch (menuPosition)
		{
		case MENU_EXIT:
			exit = true;
			break;
		case MENU_CONTROLS:
			showControls = !showControls;
			controlsBox.SetVisible(!controlsBox.IsVisible());
			break;
		default:
			if (!showMainMenu && mapLoader.GetMapList(singlePlayer).size() > previewMapIndex)
			{
				// load level
				//Level::levelName = mapLoader.LoadMap(mapLoader.GetMapList(singlePlayer)[previewMapIndex]).GetName();
				//Map m = mapLoader.LoadMap(Level::levelName);
				//SwitchScene(SCENE_LEVEL);
				Say::Log("Load level");
			}

			Say::Log("showMainMenu = false");
			showMainMenu = false;
			LoadMapPreview();
			break;
		}

		return;
	}

	if (showMainMenu)
	{
		if (key == GLFW_KEY_DOWN)
			menuPosition++;
		else if (key == GLFW_KEY_UP)
			menuPosition--;

		// make sure we arent out of range in our menu options
		if (menuPosition < 1) menuPosition = 1;
		if (menuPosition > 4) menuPosition = 4;

		singlePlayer = (menuPosition == 1 ? true : false);
	}
	else
	{
		if (key == GLFW_KEY_RIGHT)
		{
			if (mapLoader.GetMapList(singlePlayer).size() > (previewMapIndex + 1))
			{
				previewMapIndex++;
				LoadMapPreview();
			}
		}
		else if (key == GLFW_KEY_LEFT)
		{
			if (previewMapIndex > 0)
			{
				previewMapIndex--;
				LoadMapPreview();
			}
			else
			{
				mapPreview.clear();
				showMainMenu = true;
			}
		}
	}
}

void MainMenu::LoadMapPreview()
{
	Say::Log("->LoadMapPreview", singlePlayer ? "sp" : "mp", previewMapIndex);

	// clear map
	mapPreview.clear();

	float w = 1.0f;
	float h = 1.0f;
	glm::vec2 boundaries(w / 2, h / 2);

	// return if there are no maps with specified index
	if (mapLoader.GetMapList(singlePlayer).size() <= previewMapIndex)
		return;

	// get map
	Map map;
	map = mapLoader.LoadMap(mapLoader.GetMapList(singlePlayer)[previewMapIndex]);

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
		yOffset = (boundaries.x - (blockSize * yCount)) / 2;
	}

	// fill mapPreview
	int c = 0;
	for (int i = 0; i < map.GetY(); i++)
	{
		for (int j = 0; j < map.GetX(); j++)
		{
			int texture = PREVIEW_BLOCK_BACKGROUND;
			bool drawBackground = false;
			// switch to determine texture, block type accessed by map[c++]
			int b = map[c++];
			switch (b)
			{
			case BLOCK_SPACE:
				texture = PREVIEW_BLOCK_BACKGROUND;
				break;
			case BLOCK_SOLID:
				texture = PREVIEW_BLOCK_SOLID;
				break;
			case BLOCK_EXPLODABLE:
				texture = PREVIEW_BLOCK_EXPLODABLE;
				break;
			case BLOCK_PLAYER:
				texture = PREVIEW_BLOCK_BOMBERMAN;
				drawBackground = true;
				break;
			default:
				if (b >= 0 && b <= 9)
					texture = PREVIEW_BLOCK_CREEP;
				drawBackground = true;
				break;
			}

			glm::vec3 position = glm::vec3(xOffset + (boundaries.x / 2) + j * blockSize, h - (boundaries.y / 2) - i * blockSize - yOffset, 5.0f);
			glm::vec2 scale = glm::vec2(blockSize, blockSize);
			glm::vec2 pivot = glm::vec2(0.0f, 1.0f);

			if (drawBackground)
			{
				Sprite* bkg = new Sprite(position, scale, pivot);
				bkg->SetMaterial(materials[PREVIEW_BLOCK_BACKGROUND]);
				Sprite::AddToMap(bkg);

				mapPreview.push_back(*bkg);
			}

			Sprite* s = new Sprite(position, scale, pivot);
			s->SetMaterial(materials[texture]);
			Sprite::AddToMap(s);

			mapPreview.push_back(*s);
		}
	}
}