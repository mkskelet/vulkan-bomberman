#pragma once

#include "VulkanCore.h"

class VulkanApp
{
private:
	static VulkanApp* instance;
	GLFWwindow* window;
	VulkanRenderer* renderer;

	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();

public:
	VulkanApp();
	~VulkanApp() {}

	void Run();
	static VulkanApp* GetInstance() { return instance; }
	static VulkanRenderer* GetRenderer();
};