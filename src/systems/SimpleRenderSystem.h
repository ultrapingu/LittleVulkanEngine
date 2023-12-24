#pragma once

#include "Device.h"
#include "Pipeline.h"
#include "GameObject.h"
#include "Camera.h"
#include "FrameInfo.h"

#include <memory>
#include <vector>


namespace lve
{
	class SimpleRenderSystem
	{
	public:
		SimpleRenderSystem(
			Device &d, 
			VkRenderPass renderPass,
			VkDescriptorSetLayout globalSetLayout);
		~SimpleRenderSystem();

		SimpleRenderSystem(const SimpleRenderSystem&) = delete;
		SimpleRenderSystem& operator=(const SimpleRenderSystem&) = delete;

		void renderGameObjects(
			FrameInfo &frameInfo);
		
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
		
		
		Device &device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

