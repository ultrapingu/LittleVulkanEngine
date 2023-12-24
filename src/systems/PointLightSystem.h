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
	class PointLightSystem
	{
	public:
		PointLightSystem(
			Device &d, 
			VkRenderPass renderPass,
			VkDescriptorSetLayout globalSetLayout);
		~PointLightSystem();

		PointLightSystem(const PointLightSystem&) = delete;
		PointLightSystem& operator=(const PointLightSystem&) = delete;

		void render(
			FrameInfo &frameInfo);
		
	private:
		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);
		
		
		Device &device;

		std::unique_ptr<Pipeline> pipeline;
		VkPipelineLayout pipelineLayout;
	};
}

