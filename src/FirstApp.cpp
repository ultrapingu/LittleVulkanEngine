#include "FirstApp.h"

#include "systems/SimpleRenderSystem.h"
#include "systems/PointLightSystem.h"
#include "Camera.h"
#include "KeyboardMovementController.h"
#include "Buffer.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <stdexcept>
#include <chrono>


namespace lve
{
    const float MAX_FRAME_TIME = 1.0f;

    struct GlobalUbo
    {
        glm::mat4 projection{ 1.0f };
        glm::mat4 view{ 1.0f };
        glm::vec4 ambientLightColor{ 1.0f, 1.0f, 1.0f, 0.02f };
        glm::vec4 lightPosition{ 0.0f, -0.5f, 0.0f, 1.0f };
        glm::vec4 lightColor{ 1.0f };
    };

	FirstApp::FirstApp()
	{
        globalPool = DescriptorPool::Builder(device)
            .setMaxSets(SwapChain::MAX_FRAMES_IN_FLIGHT)
            .addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, SwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApp::~FirstApp() { }

	void FirstApp::run()
	{
        float runTimeSec = 0.0f;

        std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < uboBuffers.size(); ++i)
        {
            uboBuffers[i] = std::make_unique<Buffer>(
                device,
                sizeof(GlobalUbo),
                1,
                VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
            uboBuffers[i]->map();
        }

        auto globalSetLayout = DescriptorSetLayout::Builder(device)
            .addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS)
			.build();

        std::vector<VkDescriptorSet> globalDescriptorSets(SwapChain::MAX_FRAMES_IN_FLIGHT);
        for (int i = 0; i < globalDescriptorSets.size(); ++i)
        {
            auto bufferInfo = uboBuffers[i]->descriptorInfo();
            DescriptorWriter(*globalSetLayout, *globalPool)
        		.writeBuffer(0, &bufferInfo)
        		.build(globalDescriptorSets[i]);
        }

		SimpleRenderSystem simpleRenderSystem{
			device,
			renderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout()};
        PointLightSystem pointLightSystem{
			device,
			renderer.getSwapChainRenderPass(),
			globalSetLayout->getDescriptorSetLayout()};
        Camera camera{};

        auto viewerObject = GameObject::createGameObject();
        viewerObject.transform.translation = glm::vec3(0.0f, -2.0f, -10.0f);
        KeyboardMovementController cameraController{};

        auto currentTime = std::chrono::high_resolution_clock::now();

		while (!window.shouldClose())
		{
			glfwPollEvents();

            auto newTime = std::chrono::high_resolution_clock::now();
            float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
            runTimeSec += frameTime;
            currentTime = newTime;

            frameTime = glm::min(frameTime, MAX_FRAME_TIME);

            cameraController.moveInPlaneXZ(window.getGLFWwindow(), frameTime, viewerObject);
            camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

            float aspect = renderer.getAspectRatio();
            camera.setPerspectiveProjection(glm::radians(50.0f), aspect, 0.1f, 30.0f);

			if (auto commandBuffer = renderer.beginFrame())
			{
                int frameIndex = renderer.getFrameIndex();
                FrameInfo frameInfo{
                    frameIndex,
                    frameTime,
                    commandBuffer,
                    camera,
                    globalDescriptorSets[frameIndex],
                gameObjects};

                // Update
                GlobalUbo ubo{};
                ubo.projection = camera.getProjection();
                ubo.view = camera.getView();
                ubo.lightPosition = glm::vec4(glm::sin(runTimeSec), glm::sin(runTimeSec * 0.7f) * 0.5f - 1.5f, -glm::cos(runTimeSec), ubo.lightPosition.w);
                ubo.lightColor = glm::vec4(glm::sin(runTimeSec) * 0.5f + 0.5f, glm::sin(runTimeSec * 1.3f) * 0.5f + 0.5f, glm::sin(runTimeSec * 1.5f) * 0.5f + 0.5f, ubo.lightColor.w);
                uboBuffers[frameIndex]->writeToBuffer(&ubo);
                uboBuffers[frameIndex]->flush();

                // Render
				renderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(device.device());
	}

    // temporary helper function, creates a 1x1x1 cube centered at offset with an index buffer
    std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
        Model::Builder modelBuilder{};
        modelBuilder.vertices = {
            // left face (white)
            {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
            {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},

            // right face (yellow)
            {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},

            // top face (orange, remember y axis points down)
            {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
            {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},

            // bottom face (red)
            {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
            {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},

            // nose face (blue)
            {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
            {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},

            // tail face (green)
            {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
            {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
        };
        for (auto& v : modelBuilder.vertices) {
            v.position += offset;
        }

        modelBuilder.indices = { 0,  1,  2,  0,  3,  1,  4,  5,  6,  4,  7,  5,  8,  9,  10, 8,  11, 9,
                                12, 13, 14, 12, 15, 13, 16, 17, 18, 16, 19, 17, 20, 21, 22, 20, 23, 21 };

        return std::make_unique<Model>(device, modelBuilder);
    }

	void FirstApp::loadGameObjects()
	{
        // Smooth vase
        auto smoothVase = GameObject::createGameObject();
        smoothVase.model = Model::createModelFromFile(device, "models\\smooth_vase.obj");
        smoothVase.transform.translation = { -2.0f, 0.0f, 2.0f };
        smoothVase.transform.scale = glm::vec3(3.0f, 3.0f, 3.0f);
        gameObjects.emplace(smoothVase.getId(), std::move(smoothVase));

        // Flat vase
        auto flatVase = GameObject::createGameObject();
        flatVase.model = Model::createModelFromFile(device, "models\\flat_vase.obj");
        flatVase.transform.translation = { 2.0f, 0.0f, -2.0f };
        flatVase.transform.scale = glm::vec3(3.0f, 3.0f, 3.0f);
        gameObjects.emplace(flatVase.getId(), std::move(flatVase));

        // Floor model
        auto floor = GameObject::createGameObject();
        floor.model = Model::createModelFromFile(device, "models\\quad.obj");
        floor.transform.translation = { 0.0f, 0.0f, 0.0f };
        floor.transform.scale = { 3.0f, 1.0f, 3.0f };
        gameObjects.emplace(floor.getId(), std::move(floor));
	}
}
