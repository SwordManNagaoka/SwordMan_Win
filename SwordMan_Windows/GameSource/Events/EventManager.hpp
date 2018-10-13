#pragma once
#include <string>
#include <iostream>
#include "../Class/Scene/SceneManager.hpp"
namespace Event
{
	class EventManager final
	{
	private:
		class Singleton final
		{
		private:
			using SceneManager = Scene::SceneManager;
			std::vector<std::pair<SceneManager::State, std::function<void()>>> events;
		public:
			void Add(const SceneManager::State state, std::function<void()> func)
			{
				events.emplace_back(std::make_pair(state, func));
			}
			void Update()
			{
				for (auto& it : events)
				{
					if (SceneManager::Get().GetCurrentScene() == it.first)
					{
						it.second();
					}
				}
			}
		};
	public:
		static Singleton& Get()
		{
			static auto inst = std::make_unique<Singleton>();
			return *inst;
		}
	};
}
