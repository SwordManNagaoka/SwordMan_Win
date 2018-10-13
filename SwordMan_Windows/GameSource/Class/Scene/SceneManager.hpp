/**
* @file SceneManager.hpp
* @brief Sceneオブジェクトを管理します
* @author tonarinohito
* @date 2018/9/21
*/
#pragma once
#include "../../ECS/ECS.hpp"
#include "../../Utility/Utility.hpp"
#include "Scene.hpp"
#include "Game.h"
#include "Title.h"
#include "Result.h"
#include "Pause.h"
#include "Menu.h"

namespace Scene
{
	class SceneManager final
	{
	public:
		enum class State
		{
			Title,
			Game,
			Menu,
			Pause,
			Result,
		};
	private:
		class Singleton final
		{
		private:
			State state;
			ISceneBase* pScene = nullptr;
		public:
			~Singleton()
			{
				Memory::SafeDelete(pScene);
			}
			void ChangeScene(State scene)
			{
				Memory::SafeRelease(pScene);
				Memory::SafeDelete(pScene);
				state = scene;
				switch (scene)
				{
				case State::Title:	pScene = new Title();  break;
				case State::Game:	pScene = new Game();   break;
				case State::Menu:	pScene = new Menu();   break;
				case State::Pause:	pScene = new Pause();  break;
				case State::Result:	pScene = new Result(); break;	
				}
			}
			const State GetCurrentScene() const
			{
				return state;
			}
			void Update()
			{
				pScene->Update();
			}
			void Draw()
			{
				pScene->Draw();
			}
		};
	public:
		inline static Singleton& Get()
		{
			static std::unique_ptr<Singleton> inst =
				std::make_unique<Singleton>();
			return *inst;
		}
	};

}