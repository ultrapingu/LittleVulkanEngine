#pragma once

#include "Camera.h"
#include "GameObject.h"

#include <vulkan/vulkan.h>


namespace lve
{
	struct FrameInfo
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		Camera& camera;
		VkDescriptorSet globalDescriptorSet;
		GameObject::Map& gameObjects;
	};
}