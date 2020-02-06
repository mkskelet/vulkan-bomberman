#pragma once

#define SCENE_SPLASHSCREEN 0
#define SCENE_MAIN_MENU 1
#define SCENE_LEVEL 2

class Scene
{
private:
	bool switchScene;		///< Signal to switch scene.					
	int nextScene;			///< Scene to switch to.
protected:
	int sceneIndex;
public:
	bool exit;									///< sgnal for GameWindow to close the game.
	Scene(int index);
	virtual void Render();						///< Renders whole scene.
	virtual void Update();						///< Calls update function of each important object in the scene.
	virtual void KeyPress(int key);
	virtual void SpecialKeyPress(int key);
	virtual void KeyRelease(int key);
	virtual void SpecialKeyRelease(int key);
	virtual void Start();
	int GetNextScene();							///< Returns sceen that has to be created after this scene gets deleted.
	void SwitchScene(int newIndex);				///< Method sets next scene and sets switchScene to true.
	bool IsSwitchingScene();						///< Returns true if scene needs to be switched.
	virtual void OnWindowResized();				///< Called after resizing a window
};


