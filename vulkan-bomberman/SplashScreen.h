#pragma once

#include "Sprite.h"
#include "Scene.h"

class SplashScreen : public Scene
{
private:
	float timer;			///< Time before switching scene.
	Sprite background;		///< Background sprite.
	Sprite madeBy;
public:
	SplashScreen();
	void Render();
	void Start();
	void Update();
	void KeyPress(unsigned char key);
	void SpecialKeyPress(int key);
	void OnWindowResized();
};

