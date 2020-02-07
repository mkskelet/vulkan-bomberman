#include "SplashScreen.h"

#include "GameTime.h"
#include "TextureDatabase.h"
#include <iostream>

/// Constructor for main menu scene, sets player count and ai count to lowest needed for the game to start.
SplashScreen::SplashScreen() : Scene(SCENE_SPLASHSCREEN)
{
	OnWindowResized();
}

/// Function to render scene.
void SplashScreen::Render()
{
	//background.Render();
	////madeBy.Render();

	//glColor3f(0.7f, 0.0f, 1.0f);
	//glRasterPos2f(0, 0);
	//int len, i;
	//char* arr = (char*)"Made by Marek Kost\0";
	//len = (int)strlen(arr);
	//for (i = 0; i < len; i++)
	//{
	//	glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, arr[i]);
	//}
	//glColor3f(1.0f, 1.0f, 1.0f);
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
		//SwitchScene(SCENE_MAIN_MENU);
		std::cout << "LOAD NEXT SCENE!";
		timer = 1000000000;				// dont want to repeatedly call switch scene
	}
}

/// Method called when key is pressed.
void SplashScreen::KeyPress(unsigned char key)
{
	if (key == 27)
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
	background = Sprite(glm::vec3(0.5f, 0.5f, 0.5f), glm::vec2(1.0f, 1.0f), nullptr, PIVOT_CENTER);
	background.SetTexture(TextureDatabase::GetInstance().GetTexture("../sprites/splashscreen.tga"));
}

Texture* SplashScreen::GetSprite()
{
	return background.GetTexture();
}