#pragma once

#include "VulkanCore.h"

class VulkanApp
{
private:
	GLFWwindow* window;
	VulkanRenderer* renderer;

	void InitWindow();
	void InitVulkan();
	void MainLoop();
	void Cleanup();

public:
	void Run();
	static VulkanApp& GetInstance();
	static VulkanRenderer* GetRenderer();
};