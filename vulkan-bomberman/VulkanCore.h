#pragma once

#ifndef GLFW_INCLUDE_VULKAN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#endif

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/glm.hpp>

#include "Vertex.h"
#include <array>
#include <cstdlib>
#include <optional>
#include "Material.h"
#include "Sprite.h"

struct UniformBufferObject
{
	glm::mat4 model;
	glm::mat4 view;
	glm::mat4 proj;
};

const std::vector<const char*> deviceExtensions = {
	VK_KHR_SWAPCHAIN_EXTENSION_NAME
};

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	inline bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanRenderer
{
private:
	// DEBUG
	VkDebugUtilsMessengerEXT debugMessenger;

	// INSTANCE VARIABLES
	GLFWwindow* window;
	VkInstance instance;
	VkSurfaceKHR surface;
	VkQueue presentQueue;

	VkSwapchainKHR swapChain;
	VkFormat swapChainImageFormat;
	std::vector<VkImageView> swapChainImageViews;
	std::vector<VkFramebuffer> swapChainFramebuffers;

	VkQueue graphicsQueue;
	VkCommandPool commandPool;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device;

	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	std::vector<VkFence> imagesInFlight;
	size_t currentFrame = 0;

	VkRenderPass renderPass;
	VkDescriptorSetLayout descriptorSetLayout;
	VkPipelineLayout pipelineLayout;

	VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
	std::vector<VkPipeline> graphicsPipelines;

	VkExtent2D swapChainExtent;
	std::vector<VkImage> swapChainImages;

	std::vector<VkBuffer> uniformBuffers;

	VkDescriptorPool descriptorPool;
	std::vector<std::vector<VkDescriptorSet>> descriptorSets;

	std::vector<VkCommandBuffer> commandBuffers;

	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;

	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;

	std::vector<VkDeviceMemory> uniformBuffersMemory;

	VkImage depthImage;
	VkDeviceMemory depthImageMemory;
	VkImageView depthImageView;

	VkImage colorImage;
	VkDeviceMemory colorImageMemory;
	VkImageView colorImageView;

	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;

	// RENDERING VARIABLES
	std::map<int, Material*> spriteMaterialMap;

	// PRIVATE METHODS
	void CreateInstance();
	
	bool CheckValidationLayerSupport();
	void SetupDebugMessenger();
	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator);
	void PopulateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
	static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
	
	std::vector<const char*> GetRequiredExtensions();
	bool CheckExtensionSupport(std::vector<VkExtensionProperties> * extensions, uint32_t glfwExtensionCount, std::vector<const char*> glfwExtensions);

	void CreateSurface();

	void PickPhysicalDevice();
	bool IsDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices FindQueueFamilies(VkPhysicalDevice device);
	bool CheckDeviceExtensionSupport(VkPhysicalDevice device);
	VkSampleCountFlagBits GetMaxUsableSampleCount();
	SwapChainSupportDetails QuerySwapChainSupport(VkPhysicalDevice device);

	void CreateLogicalDevice();

	void CreateSwapChain();
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR ChooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR ChooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
	VkExtent2D ChooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
	void CreateSwapChainImageViews();

	void CreateRenderPass();
	VkFormat FindDepthFormat();
	VkFormat FindSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
	void CreateDescriptorSetLayout();
	void CreateCommandPool();
	void CreateColorResources();
	void CreateDepthResources();
	void CreateFramebuffers();
	void CreateUniformBuffers();
	void CreateDescriptorPool();
	void CreateSyncObjects();
	void RecreateSwapChain();
	void FillIndexBuffer();
	bool RenderSprite(Sprite* sprite);
	void CreateVertexBuffer();
	void CreateIndexBuffer();
	void CopyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
	void DrawFrame();
	void UpdateUniformBuffer(uint32_t currentImage);
	void CreateCommandBuffers();
	void CleanupSwapChain();

	void GenerateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
	void CreateImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
	VkImageView CreateImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
	VkCommandBuffer BeginSingleTimeCommands();
	void EndSingleTimeCommands(VkCommandBuffer commandBuffer);
	void TransitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
	void CopyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
	void CreateBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
	bool HasStencilComponent(VkFormat format);
	VkShaderModule CreateShaderModule(const std::vector<char>& code);

public:
	bool framebufferResized = false;

	VulkanRenderer(GLFWwindow* window);
	~VulkanRenderer();
	void InitVulkan();
	void Render();
	inline VkDevice GetDevice() const { return device; }

	// texture creation
	void CreateTextureImage(const char* path, VkImage* image, VkDeviceMemory* imageMemory, bool useMipMaps = false, uint32_t* mipLevels = nullptr);
	void CreateTextureImageView(VkImageView* imageView, VkImage* image, uint32_t mipLevels);
	void CreateTextureSampler(VkSampler* sampler, uint32_t mipLevels);

	// shaders and material creation
	void CreateGraphicsPipeline(int& index, const char* vertexShaderPath, const char* fragmentShaderPath);
	void CreateDescriptorSets(VkImageView* image, VkSampler* sampler, int& index);
	void UpdateDescriptorSets(std::vector<VkDescriptorSet>* descriptorSets, VkImageView* image, VkSampler* sampler);
};