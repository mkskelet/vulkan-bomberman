#pragma once

#include "VulkanCore.h"

class Texture
{
private:
	uint32_t textureMipLevels;
	VkImage textureImage;
	VkDeviceMemory textureImageMemory;
	const char* path;
	uint32_t refCount = 0;
	
public:
	VkImageView textureImageView;
	VkSampler textureSampler;

	Texture(const char* path);
	~Texture();
	const char* GetPath() const { return path; }
	void IncrementRefCount() { refCount++; }
	void DecrementRefCount() { refCount--; }
	uint32_t GetRefCount() { return refCount; }
};

