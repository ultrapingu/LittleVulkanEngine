#pragma once

#include "Window.h"
#include "Device.h"
#include "GameObject.h"
#include "Renderer.h"
#include "Descriptor.h"

#include <memory>
#include <vector>


namespace lve
{
	class FirstApp
	{
	public:
		static constexpr int WIDTH = 800;
		static constexpr int HEIGHT = 800;

		FirstApp();
		~FirstApp();

		FirstApp(const FirstApp&) = delete;
		FirstApp &operator=(const FirstApp&) = delete;

		void run();

	private:
		void loadGameObjects();

		Window window{ WIDTH, HEIGHT, "First App" };
		Device device{ window };
		Renderer renderer{window, device};

		std::unique_ptr<DescriptorPool> globalPool{};
		GameObject::Map gameObjects;
	};
}

