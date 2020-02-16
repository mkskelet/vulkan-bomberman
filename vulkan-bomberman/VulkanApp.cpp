#include "VulkanApp.h"

#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <algorithm>
#include <array>
#include "GameTime.h"
#include "Scene.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "TextureDatabase.h"

const int WIDTH = 800;
const int HEIGHT = 800;

// begin Debug
float viewX = 0.5f;
float viewY = 0.5f;
float viewZ = 0.0f;

Scene* scene = nullptr; // TEMP global

void moveView(float x, float y, float z)
{
	viewX += x;
	viewY += y;
	viewZ += z;

	std::cout << "x " << viewX << ", y " << viewY << ",z " << viewZ << std::endl;
}
// end

// begin Debug
void ProcessDebugCommands(int key)
{
	switch (key)
	{
	case GLFW_KEY_F1:
	{
		// debug Sprite map
		/*auto smap = Sprite::GetSpriteMap();

		std::cout << std::endl << "Sprite map debug: " << std::endl;
		for (const auto& [tex, vec] : smap)
		{
			std::cout << (tex != nullptr ? tex->GetPath() : "null") << ", ref count: " << vec.size() << std::endl;
		}

		break;*/
	}
	default:
		break;
	}
}
// end

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	float x = 0, y = 0, z = 0;

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		y = 0.5f;
	}
	else if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		y = -0.5f;
	}
	else if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		x = -0.5f;
	}
	else if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		x = 0.5f;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		z = -0.5f;
	}
	else if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		z = 0.5f;
	}

	//moveView(x, y, z);

	if (scene != nullptr && action == GLFW_PRESS)
	{
		scene->KeyPress(key);

		ProcessDebugCommands(key);
	}
}

void VulkanApp::Run()
{
	InitWindow();
	InitVulkan();
	MainLoop();
	Cleanup();
}

void VulkanApp::InitWindow()
{
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

	window = glfwCreateWindow(WIDTH, HEIGHT, "Vulkan", nullptr, nullptr);
	glfwSetWindowUserPointer(window, this);
	glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
}

static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<VulkanApp*>(glfwGetWindowUserPointer(window));
	app->GetRenderer()->framebufferResized = true;
}

void VulkanApp::InitVulkan()
{
	renderer = new VulkanRenderer(window);
	renderer->InitVulkan();
}

void VulkanApp::MainLoop()
{
	GameTime gameTime = GameTime(std::chrono::high_resolution_clock::now());

	TextureDatabase texDB;

	glfwSetKeyCallback(window, keyCallback);

	scene = new SplashScreen();
	scene->Start();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		gameTime.UpdateTime(std::chrono::high_resolution_clock::now());
		scene->Update();

		if (scene->exit)
		{
			break;
		}
		else if (scene->IsSwitchingScene())
		{
			int newScene = scene->GetNextScene();
			delete scene;

			switch (newScene)
			{
			case SCENE_MAIN_MENU:
				scene = new MainMenu();
				scene->Start();
				break;
			default:
				break;
			}
		}

		renderer->Render();
	}

	vkDeviceWaitIdle(renderer->GetDevice()); // finish rendering

	texDB.ReleaseTextures();
	delete scene;
}

void VulkanApp::Cleanup()
{
	delete renderer;
	glfwDestroyWindow(window);
	glfwTerminate();
}

VulkanApp& VulkanApp::GetInstance()
{
	static VulkanApp instance;
	return instance;
}

VulkanRenderer* VulkanApp::GetRenderer()
{
	return GetInstance().renderer;
}