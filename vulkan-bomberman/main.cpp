#include <iostream>
#include <stdexcept>
#include <functional>
#include <cstdlib>
#include <string>
#include <cstdint>
#include <algorithm>
#include <array>

#include "VulkanCore.h"
#include "GameTime.h"
#include "Scene.h"
#include "SplashScreen.h"
#include "MainMenu.h"
#include "TextureDatabase.h"

const int MAX_FRAMES_IN_FLIGHT = 2;

const int WIDTH = 800;
const int HEIGHT = 800;

// begin Debug
float viewX = 0.5f;
float viewY = 0.5f;
float viewZ = 0.0f;

Scene* scene = nullptr;

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

class VulkanApp
{
public:
	void run()
	{
		initWindow();

		initVulkan(); // VulkanCore.InitVulkan


		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;
	VulkanRenderer* renderer;

	void initWindow()
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
		app->framebufferResized = true;
	}

	void initVulkan()
	{
		renderer = new VulkanRenderer(window);
		renderer->InitVulkan();

		//createInstance();
		//createSurface();
		//pickPhysicalDevice();
		//createLogicalDevice();
		//createSwapChain();
		createImageViews();
		createRenderPass();
		createDescriptorSetLayout();

		Shader uberShader = Shader("uber", "../shaders/vert.spv", "../shaders/frag.spv");

		createCommandPool();
		createColorResources();
		createDepthResources();
		createFramebuffers();
		createUniformBuffers();
		createDescriptorPool();

		//VulkanRenderer::GetInstance().descriptorSets.push_back({});
		//createDescriptorSets(&descriptorSets[0], nullptr, nullptr);

		createSyncObjects();
	}

	void recreateSwapChain()
	{
		int width = 0, height = 0;
		while (width == 0 || height == 0)
		{
			glfwGetFramebufferSize(window, &width, &height);
			glfwWaitEvents();
		}

		vkDeviceWaitIdle(VulkanRenderer::GetInstance().device);

		cleanupSwapChain();

		createSwapChain();
		createImageViews();
		createRenderPass();
		createColorResources();
		createDepthResources();
		createFramebuffers();
		createUniformBuffers();
		createDescriptorPool();
		createCommandBuffers();
	}

	void createImageViews()
	{
		swapChainImageViews.resize(VulkanRenderer::GetInstance().swapChainImages.size());

		for (uint32_t i = 0; i < VulkanRenderer::GetInstance().swapChainImages.size(); i++)
		{
			swapChainImageViews[i] = createImageView(VulkanRenderer::GetInstance().swapChainImages[i], swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
		}
	}

	void createDescriptorSetLayout()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		std::array<VkDescriptorSetLayoutBinding, 2> bindings = { uboLayoutBinding, samplerLayoutBinding };
		VkDescriptorSetLayoutCreateInfo layoutInfo = {};
		layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
		layoutInfo.pBindings = bindings.data();

		if (vkCreateDescriptorSetLayout(VulkanRenderer::GetInstance().device, &layoutInfo, nullptr, &VulkanRenderer::GetInstance().descriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create descriptor set layout!");
		}
	}

	void createRenderPass()
	{
		// color attachment
		VkAttachmentDescription colorAttachment = {};
		colorAttachment.format = swapChainImageFormat;
		colorAttachment.samples = VulkanRenderer::GetInstance().msaaSamples;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkAttachmentReference colorAttachmentRef = {};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// color attachment to resolve multisampling
		VkAttachmentDescription colorAttachmentResolve = {};
		colorAttachmentResolve.format = swapChainImageFormat;
		colorAttachmentResolve.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachmentResolve.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachmentResolve.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachmentResolve.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachmentResolve.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachmentResolve.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentResolveRef = {};
		colorAttachmentResolveRef.attachment = 2;
		colorAttachmentResolveRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// depth attachment
		VkAttachmentDescription depthAttachment = {};
		depthAttachment.format = findDepthFormat();
		depthAttachment.samples = VulkanRenderer::GetInstance().msaaSamples;
		depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		VkAttachmentReference depthAttachmentRef = {};
		depthAttachmentRef.attachment = 1;
		depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

		// subpass
		VkSubpassDescription subpass = {};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;
		subpass.pDepthStencilAttachment = &depthAttachmentRef;
		subpass.pResolveAttachments = &colorAttachmentResolveRef;

		// subpass dependency
		VkSubpassDependency dependency = {};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		// create renderPass
		std::array<VkAttachmentDescription, 3> attachments = { colorAttachment, depthAttachment, colorAttachmentResolve };
		VkRenderPassCreateInfo renderPassInfo = {};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
		renderPassInfo.pAttachments = attachments.data();
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;

		if (vkCreateRenderPass(VulkanRenderer::GetInstance().device, &renderPassInfo, nullptr, &VulkanRenderer::GetInstance().renderPass) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create render pass!");
		}
	}

	void createFramebuffers()
	{
		swapChainFramebuffers.resize(swapChainImageViews.size());

		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			std::array<VkImageView, 3> attachments = {
				colorImageView,
				depthImageView,
				swapChainImageViews[i]
			};

			VkFramebufferCreateInfo framebufferInfo = {};
			framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferInfo.renderPass = VulkanRenderer::GetInstance().renderPass;
			framebufferInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
			framebufferInfo.pAttachments = attachments.data();
			framebufferInfo.width = VulkanRenderer::GetInstance().swapChainExtent.width;
			framebufferInfo.height = VulkanRenderer::GetInstance().swapChainExtent.height;
			framebufferInfo.layers = 1;

			if (vkCreateFramebuffer(VulkanRenderer::GetInstance().device, &framebufferInfo, nullptr, &swapChainFramebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to create framebuffer!");
			}
		}
	}

	void createCommandPool()
	{
		QueueFamilyIndices queueFamilyIndices = findQueueFamilies(VulkanRenderer::GetInstance().physicalDevice);

		VkCommandPoolCreateInfo poolInfo = {};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();
		poolInfo.flags = 0; // Optional

		if (vkCreateCommandPool(VulkanRenderer::GetInstance().device, &poolInfo, nullptr, &VulkanRenderer::GetInstance().commandPool) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to create command pool!");
		}
	}

	void createColorResources() 
	{
		VkFormat colorFormat = swapChainImageFormat;

		createImage(VulkanRenderer::GetInstance().swapChainExtent.width, VulkanRenderer::GetInstance().swapChainExtent.height, 1, VulkanRenderer::GetInstance().msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
		colorImageView = createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
	}

	void createDepthResources()
	{
		VkFormat depthFormat = findDepthFormat();
		createImage(VulkanRenderer::GetInstance().swapChainExtent.width, VulkanRenderer::GetInstance().swapChainExtent.height, 1, VulkanRenderer::GetInstance().msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
		depthImageView = createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

		transitionImageLayout(depthImage, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL, 1);
	}

	VkFormat findDepthFormat()
	{
		return findSupportedFormat(
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);
	}

	VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
	{
		for (VkFormat format : candidates)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(VulkanRenderer::GetInstance().physicalDevice, format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return format;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return format;
			}
		}

		throw std::runtime_error("failed to find supported format!");
	}
	
	void createVertexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory);

        createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

        copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

        vkDestroyBuffer(VulkanRenderer::GetInstance().device, stagingBuffer, nullptr);
        vkFreeMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory, nullptr);
	}

	void createIndexBuffer()
	{
		VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

		VkBuffer stagingBuffer;
		VkDeviceMemory stagingBufferMemory;
		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

		void* data;
		vkMapMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory, 0, bufferSize, 0, &data);
		memcpy(data, indices.data(), (size_t)bufferSize);
		vkUnmapMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory);

		createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

		copyBuffer(stagingBuffer, indexBuffer, bufferSize);

		vkDestroyBuffer(VulkanRenderer::GetInstance().device, stagingBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::GetInstance().device, stagingBufferMemory, nullptr);
	}

	void createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		VulkanRenderer::GetInstance().uniformBuffers.resize(VulkanRenderer::GetInstance().swapChainImages.size());
		uniformBuffersMemory.resize(VulkanRenderer::GetInstance().swapChainImages.size());

		for (size_t i = 0; i < VulkanRenderer::GetInstance().swapChainImages.size(); i++)
		{
			createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, VulkanRenderer::GetInstance().uniformBuffers[i], uniformBuffersMemory[i]);
		}
	}

	void createCommandBuffers()
	{
		commandBuffers.resize(swapChainFramebuffers.size());

		VkCommandBufferAllocateInfo allocInfo = {};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = VulkanRenderer::GetInstance().commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = (uint32_t)commandBuffers.size();

		if (vkAllocateCommandBuffers(VulkanRenderer::GetInstance().device, &allocInfo, commandBuffers.data()) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to allocate command buffers!");
		}

		// record command buffers
		for (size_t i = 0; i < commandBuffers.size(); i++)
		{
			VkCommandBufferBeginInfo beginInfo = {};
			beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			beginInfo.flags = 0; // Optional
			beginInfo.pInheritanceInfo = nullptr; // Optional

			if (vkBeginCommandBuffer(commandBuffers[i], &beginInfo) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to begin recording command buffer!");
			}

			// clear values
			std::array<VkClearValue, 2> clearValues = {};
			clearValues[0].color = { 0.0f, 0.0f, 0.0f, 1.0f };
			clearValues[1].depthStencil = { 1.0f, 0 };

			// start a render pass
			VkRenderPassBeginInfo renderPassInfo = {};
			renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			renderPassInfo.renderPass = VulkanRenderer::GetInstance().renderPass;
			renderPassInfo.framebuffer = swapChainFramebuffers[i];
			renderPassInfo.renderArea.offset = { 0, 0 };
			renderPassInfo.renderArea.extent = VulkanRenderer::GetInstance().swapChainExtent;
			renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
			renderPassInfo.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

			if (VulkanRenderer::GetInstance().graphicsPipelines.size() > 0)
			{
				// TODO bind proper pipeline
				vkCmdBindPipeline(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanRenderer::GetInstance().graphicsPipelines[0]);

				// bind buffers
				VkBuffer vertexBuffers[] = { vertexBuffer };
				VkDeviceSize offsets[] = { 0 };

				if (spriteMaterialMap.size() > 0)
				{
					vkCmdBindVertexBuffers(commandBuffers[i], 0, 1, vertexBuffers, offsets);
					vkCmdBindIndexBuffer(commandBuffers[i], indexBuffer, 0, VK_INDEX_TYPE_UINT32);

					int pendingIndex = 0;
					Material* pendingMat = nullptr;

					int di = 0;

					for (auto const& [index, mat] : spriteMaterialMap)
					{
						if (pendingMat != nullptr && index != pendingIndex)
						{
							vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanRenderer::GetInstance().pipelineLayout, 0, 1, &VulkanRenderer::GetInstance().descriptorSets[mat->GetDescriptorSetIndex()][i], 0, nullptr);
							vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(index - pendingIndex), 1, pendingIndex, 0, 0);
						}

						pendingIndex = index;
						pendingMat = mat;
					}

					if (pendingMat != nullptr)
					{
						vkCmdBindDescriptorSets(commandBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, VulkanRenderer::GetInstance().pipelineLayout, 0, 1, &VulkanRenderer::GetInstance().descriptorSets[pendingMat->GetDescriptorSetIndex()][i], 0, nullptr);
						vkCmdDrawIndexed(commandBuffers[i], static_cast<uint32_t>(indices.size() - pendingIndex), 1, pendingIndex, 0, 0);
					}
				}
			}

			vkCmdEndRenderPass(commandBuffers[i]);

			if (vkEndCommandBuffer(commandBuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("failed to record command buffer!");
			}
		}
	}

	void createSyncObjects()
	{
		imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		imagesInFlight.resize(VulkanRenderer::GetInstance().swapChainImages.size(), VK_NULL_HANDLE);

		VkSemaphoreCreateInfo semaphoreInfo = {};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkFenceCreateInfo fenceInfo = {};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			if (vkCreateSemaphore(VulkanRenderer::GetInstance().device, &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
				vkCreateSemaphore(VulkanRenderer::GetInstance().device, &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
				vkCreateFence(VulkanRenderer::GetInstance().device, &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS)
			{

				throw std::runtime_error("failed to create semaphores!");
			}
		}
	}

	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size)
	{
		VkCommandBuffer commandBuffer = beginSingleTimeCommands();

		VkBufferCopy copyRegion = {};
		copyRegion.size = size;
		vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

		endSingleTimeCommands(commandBuffer);
	}

	std::map<int, Material*> spriteMaterialMap;

	void mainLoop()
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

			// free memory
			vkFreeCommandBuffers(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

			vkDestroyBuffer(VulkanRenderer::GetInstance().device, indexBuffer, nullptr);
			vkFreeMemory(VulkanRenderer::GetInstance().device, indexBufferMemory, nullptr);

			vkDestroyBuffer(VulkanRenderer::GetInstance().device, vertexBuffer, nullptr);
			vkFreeMemory(VulkanRenderer::GetInstance().device, vertexBufferMemory, nullptr);
			
			// pre render
			spriteMaterialMap.clear();
			FillIndexBuffer(spriteMaterialMap);

			if (vertices.size() > 0)
			{
				createVertexBuffer();
				createIndexBuffer();
				createCommandBuffers();

				// render
				drawFrame();
			}
		}

		vkDeviceWaitIdle(VulkanRenderer::GetInstance().device);

		texDB.ReleaseTextures();

		std::cout << "delete scene" << std::endl;
		delete scene;
	}

	void FillIndexBuffer(std::map<int, Material*>& map)
	{
		vertices.resize(0);
		indices.resize(0);

		int di = 0;

		for (auto const& [mat, sprites] : Sprite::GetSpriteMap())
		{
			if (mat == nullptr || mat->GetTexture())
			{
				continue;
			}

			int index = indices.size();
			bool renderedAny = false;

			for (auto const& it : sprites)
			{
				bool result = RenderSprite(it);

				renderedAny = !renderedAny ? result : renderedAny;
			}

			if (renderedAny)
			{
				map[index] = mat;
			}
		}
	}

	bool RenderSprite(Sprite * sprite)
	{
		if (!sprite->IsVisible()) return false;

		glm::vec3 position = sprite->GetPosition();
		glm::vec2 pivot = sprite->GetPivot();
		glm::vec2 scale = sprite->GetScale();
		glm::vec2 tiling = sprite->GetSpriteTiling();
		Color color = sprite->GetColor();

		float absScaleX = abs(scale.x);
		float absScaleY = abs(scale.y);
		
		float tilingX1 = tiling.x >= 0 ? 0.0f : abs(tiling.x);
		float tilingX2 = tiling.x >= 0 ? tiling.x : 0.0f;
		float tilingY1 = tiling.y >= 0 ? 0.0f : abs(tiling.y);
		float tilingY2 = tiling.y >= 0 ? tiling.y : 0.0f;

		int firstIndex = vertices.size();

		vertices.push_back({ { position.x - (pivot.x * absScaleX), position.y - (pivot.y * absScaleY), position.z}, { color.r, color.g, color.b }, { tilingX1, tilingY1 } });
		vertices.push_back({ { position.x - (pivot.x * absScaleX) + absScaleX, position.y - (pivot.y * absScaleY), position.z}, { color.r, color.g, color.b }, { tilingX2, tilingY1 } });
		vertices.push_back({ { position.x - (pivot.x * absScaleX) + absScaleX, position.y - (pivot.y * absScaleY) + absScaleY, position.z}, { color.r, color.g, color.b }, { tilingX2, tilingY2 } });
		vertices.push_back({ { position.x - (pivot.x * absScaleX), position.y - (pivot.y * absScaleY) + absScaleY, position.z}, { color.r, color.g, color.b }, { tilingX1, tilingY2 } });

		indices.push_back(firstIndex);
		indices.push_back(firstIndex + 1);
		indices.push_back(firstIndex + 2);
		indices.push_back(firstIndex + 2);
		indices.push_back(firstIndex + 3);
		indices.push_back(firstIndex);

		return true;
	}

	void cleanupSwapChain()
	{
		vkDestroyImageView(VulkanRenderer::GetInstance().device, colorImageView, nullptr);
		vkDestroyImage(VulkanRenderer::GetInstance().device, colorImage, nullptr);
		vkFreeMemory(VulkanRenderer::GetInstance().device, colorImageMemory, nullptr);

		vkDestroyImageView(VulkanRenderer::GetInstance().device, depthImageView, nullptr);
		vkDestroyImage(VulkanRenderer::GetInstance().device, depthImage, nullptr);
		vkFreeMemory(VulkanRenderer::GetInstance().device, depthImageMemory, nullptr);

		for (size_t i = 0; i < swapChainFramebuffers.size(); i++)
		{
			vkDestroyFramebuffer(VulkanRenderer::GetInstance().device, swapChainFramebuffers[i], nullptr);
		}

		vkFreeCommandBuffers(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().commandPool, static_cast<uint32_t>(commandBuffers.size()), commandBuffers.data());

		//vkDestroyPipeline(VulkanRenderer::GetInstance().device, graphicsPipeline, nullptr);
		vkDestroyPipelineLayout(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().pipelineLayout, nullptr);
		vkDestroyRenderPass(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().renderPass, nullptr);

		for (size_t i = 0; i < swapChainImageViews.size(); i++)
		{
			vkDestroyImageView(VulkanRenderer::GetInstance().device, swapChainImageViews[i], nullptr);
		}

		vkDestroySwapchainKHR(VulkanRenderer::GetInstance().device, swapChain, nullptr);

		for (size_t i = 0; i < VulkanRenderer::GetInstance().swapChainImages.size(); i++)
		{
			vkDestroyBuffer(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().uniformBuffers[i], nullptr);
			vkFreeMemory(VulkanRenderer::GetInstance().device, uniformBuffersMemory[i], nullptr);
		}

		vkDestroyDescriptorPool(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().descriptorPool, nullptr);
	}

	void cleanup()
	{
		cleanupSwapChain();

		vkDestroyDescriptorSetLayout(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().descriptorSetLayout, nullptr);

		vkDestroyBuffer(VulkanRenderer::GetInstance().device, indexBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::GetInstance().device, indexBufferMemory, nullptr);

		vkDestroyBuffer(VulkanRenderer::GetInstance().device, vertexBuffer, nullptr);
		vkFreeMemory(VulkanRenderer::GetInstance().device, vertexBufferMemory, nullptr);

		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++)
		{
			vkDestroySemaphore(VulkanRenderer::GetInstance().device, renderFinishedSemaphores[i], nullptr);
			vkDestroySemaphore(VulkanRenderer::GetInstance().device, imageAvailableSemaphores[i], nullptr);
			vkDestroyFence(VulkanRenderer::GetInstance().device, inFlightFences[i], nullptr);
		}

		vkDestroyCommandPool(VulkanRenderer::GetInstance().device, VulkanRenderer::GetInstance().commandPool, nullptr);

		vkDestroyDevice(VulkanRenderer::GetInstance().device, nullptr);

		if (enableValidationLayers)
		{
			DestroyDebugUtilsMessengerEXT(instance, debugMessenger, nullptr);
		}

		vkDestroySurfaceKHR(instance, surface, nullptr);
		vkDestroyInstance(instance, nullptr);

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	void drawFrame()
	{
		vkWaitForFences(VulkanRenderer::GetInstance().device, 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);

		uint32_t imageIndex;
		VkResult result = vkAcquireNextImageKHR(VulkanRenderer::GetInstance().device, swapChain, UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

		if (result == VK_ERROR_OUT_OF_DATE_KHR)
		{
			recreateSwapChain();
			return;
		}
		else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
		{
			throw std::runtime_error("failed to acquire swap chain image!");
		}

		// update uniform buffer
		updateUniformBuffer(imageIndex);

		// Check if a previous frame is using this image (i.e. there is its fence to wait on)
		if (imagesInFlight[imageIndex] != VK_NULL_HANDLE)
		{
			vkWaitForFences(VulkanRenderer::GetInstance().device, 1, &imagesInFlight[imageIndex], VK_TRUE, UINT64_MAX);
		}

		// Mark the image as now being in use by this frame
		imagesInFlight[imageIndex] = inFlightFences[currentFrame];

		// submit command buffer
		VkSubmitInfo submitInfo = {};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

		VkSemaphore waitSemaphores[] = { imageAvailableSemaphores[currentFrame] };
		VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
		submitInfo.waitSemaphoreCount = 1;
		submitInfo.pWaitSemaphores = waitSemaphores;
		submitInfo.pWaitDstStageMask = waitStages;

		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &commandBuffers[imageIndex];

		VkSemaphore signalSemaphores[] = { renderFinishedSemaphores[currentFrame] };
		submitInfo.signalSemaphoreCount = 1;
		submitInfo.pSignalSemaphores = signalSemaphores;

		vkResetFences(VulkanRenderer::GetInstance().device, 1, &inFlightFences[currentFrame]);

		if (vkQueueSubmit(VulkanRenderer::GetInstance().graphicsQueue, 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS)
		{
			throw std::runtime_error("failed to submit draw command buffer!");
		}

		// presentation
		VkPresentInfoKHR presentInfo = {};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = signalSemaphores;

		VkSwapchainKHR swapChains[] = { swapChain };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapChains;
		presentInfo.pImageIndices = &imageIndex;
		presentInfo.pResults = nullptr; // Optional

		result = vkQueuePresentKHR(presentQueue, &presentInfo);

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR || framebufferResized)
		{
			framebufferResized = false;
			recreateSwapChain();
		}
		else if (result != VK_SUCCESS)
		{
			throw std::runtime_error("failed to present swap chain image!");
		}

		currentFrame = (currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;
	}

	void updateUniformBuffer(uint32_t currentImage)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

		UniformBufferObject ubo = {};
		ubo.model = glm::mat4(1.0f);

		ubo.view = glm::lookAt(glm::vec3(viewX, viewY, viewZ) - glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(viewX, viewY, viewZ), glm::vec3(0.0f, 1.0f, 0.0f));

		ubo.proj = glm::ortho(-0.5f, 0.5f, -0.5f, 0.5f, 0.0f, 100.f);
		ubo.proj[0][0] *= -1;

		void* data;
		vkMapMemory(VulkanRenderer::GetInstance().device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(VulkanRenderer::GetInstance().device, uniformBuffersMemory[currentImage]);
	}
};

int main()
{
	VulkanApp app;

	try
	{
		app.run();
	}
	catch (const std::exception & e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}