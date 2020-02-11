#include "SplashScreen.h"

#include "GameTime.h"
#include "TextureDatabase.h"
#include <iostream>

/// Constructor for main menu scene, sets player count and ai count to lowest needed for the game to start.
SplashScreen::SplashScreen() : Scene(SCENE_SPLASHSCREEN)
{
	OnWindowResized();
}

SplashScreen::~SplashScreen()
{

}

/// Function to render scene.
void SplashScreen::Render()
{

}

/// Method called when scene is created.
void SplashScreen::Start()
{
	timer = GameTime::Instance->GetTime() + 2000;	// set timer to 2000 miliseconds after current time
}

/// Function to update scene.
void SplashScreen::Update()
{
	if (timer <= GameTime::Instance->GetTime())
	{
		SwitchScene(SCENE_MAIN_MENU);
		std::cout << "LOAD NEXT SCENE!" << std::endl;
		timer = 1000000000;				// dont want to repeatedly call switch scene
	}
}

/// Method called when key is pressed.
void SplashScreen::KeyPress(int key)
{
	if (key == GLFW_KEY_ESCAPE)
	{
		exit = true;
	}
	else SwitchScene(SCENE_MAIN_MENU);
}

/// Method called when special key is pressed.
void SplashScreen::SpecialKeyPress(int key)
{
	SwitchScene(SCENE_MAIN_MENU);
}

/// Method called after window has been resized
void SplashScreen::OnWindowResized()
{
	background = Sprite(glm::vec3(0.5f, 0.5f, 20.0f), glm::vec2(1.0f, 1.0f), glm::vec2(0.5f, 0.5f));
	background.SetTexture(TextureDatabase::GetInstance().GetTexture("../sprites/splashscreen.tga"));
	Sprite::AddToMap(&background);
}