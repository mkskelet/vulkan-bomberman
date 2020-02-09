#pragma once

#include "Scene.h"
#include "Sprite.h"
#include "MapLoader.h"
#include <vector>
#include <string>

// menu button constants
#define SINGLE_PLAYER 1
#define MULTI_PLAYER 2
#define MENU_CONTROLS 3
#define MENU_EXIT 4

// potentialy unused
#define MIN_AI 0
#define MAX_AI 3
#define MIN_CHARACTERS 2
#define MAX_CHARACTERS 4

class MainMenu : public Scene
{
private:
	std::string mapName;	///< name of a map to load
	bool showControls;		///< Determines wether to show controls or not.
	bool singlePlayer;		///< sets game for single player if true or multi player if false
	int menuPosition;		///< position in menu (1-3 / SINGLE_PLAYER, MULTI_PLAYER, MENU_EXIT)
	bool showMainMenu;	///< controls main menu (true shows main menu and false shows level select)	
	MapLoader mapLoader;
	Sprite background;
	Sprite spButton;
	Sprite mpButton;
	Sprite exitButton;
	Sprite controlsButton;
	Sprite controlsBox;
	Sprite arrowPrev;
	Sprite arrowNext;
	std::vector<Sprite> mapPreview;		///< Map preview.
	int previewMapIndex;				///< Index of current map.
public:
	MainMenu();
	std::string GetMapName() { return mapName; }	///< Method returns name of the map selected by player.
	void Render();								///< Method renders main menu.		
	void Update();
	void KeyPress(int key);
	void Start();
	void OnWindowResized();
	//void LoadMapPreview();						///< Method loads preview map.
};