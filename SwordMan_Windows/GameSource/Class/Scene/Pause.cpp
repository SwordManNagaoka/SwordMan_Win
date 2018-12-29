#include "Pause.h"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"
//アーキタイプ
#include "../../ArcheType/Button.hpp"
//コンポーネント
#include "../../Components/BlendMode.hpp"
#include "../../Class/TouchInput.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Events/ContinueTap.hpp"
#include "../../Components/ComponentDatas/ButtonTag.hpp"
#include "../../Components/EasingMove.hpp"
#include "../../Class/Sound.hpp"

#include "../../Utility/Input.hpp"

namespace Scene
{
	Pause::Pause(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		:AbstractScene(sceneTitleChange)
	{
		//ボタン生成
		for (int i = 0; i < 3; ++i)
		{
			ECS::Entity* e = ECS::ButtonArcheType()("pauseUI", Vec2(400 + i * 150, 400), Vec2(i * 96, 0), Vec2(96, 144), 50);
			e->GetComponent<ECS::CircleColiider>().SetOffset(48, 48);
			e->AddGroup(ENTITY_GROUP::PauseUI);
			switch (i)
			{
			case 0: e->AddComponent<ECS::RetryButtonTag>(); break;
			case 1: e->AddComponent<ECS::ContinueButtonTag>(); break;
			case 2: e->AddComponent<ECS::RetryButtonTag>(); break;
			}
		}
		//ポーズ文字画像生成
		for (int i = 0; i < 5; ++i)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<ECS::Position>(Vec2(1300 + i * 170, 150));
			entity->AddComponent<ECS::RectDraw>("pauseMessage", i * 144, 0, 144, 180);
			entity->AddComponent<ECS::EasingMove>(Easing::QuartOut, ECS::EasingMove::DirectionState::LeftAndRight);
			entity->GetComponent<ECS::EasingMove>().SetBeginToEndPoint(1300 + i * 170, 220 + i * 170);
			entity->GetComponent<ECS::EasingMove>().SetTimeToDuration(30, 60);
			entity->AddGroup(ENTITY_GROUP::PauseUI);
		}
		//フェード画像生成
		ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
		entity->AddComponent<ECS::Position>();
		entity->AddComponent<ECS::SimpleDraw>("fade");
		entity->AddComponent<ECS::BlendMode>().SetAlpha(128);
		entity->AddGroup(ENTITY_GROUP::Fade1);
	}
	Pause::~Pause()
	{
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PauseUI);
		for (auto& b : button)
		{
			b->Destroy();
		}
		const auto& fade = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade1);
		for (auto& f : fade)
		{
			f->Destroy();
		}
		
	}
	void Pause::Update()
	{
		if (Input::Get().GetKeyFrame(KEY_INPUT_A) == 1)
		{
			ResourceManager::GetSound().Remove("BGM");
			GetCallback().OnSceneChange(SceneName::Game, nullptr, SceneStack::AllClear);
			return;
		}
		else if (Input::Get().GetKeyFrame(KEY_INPUT_S) == 1)
		{
			CommonData::CurrentScene::val = Scene::SceneName::Game;
			GetCallback().OnSceneChange(SceneName::BackToScene, nullptr, SceneStack::OneClear);
			return;
		}
		else if (Input::Get().GetKeyFrame(KEY_INPUT_D) == 1)
		{
			ResourceManager::GetSound().Remove("BGM");
			GetCallback().OnSceneChange(SceneName::Menu, nullptr, SceneStack::AllClear);
			return;
		}
		const auto& button = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PauseUI);

		for (auto& b : button)
		{
			b->Update();
			if (b->HasComponent<ECS::RetryButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					ResourceManager::GetSound().Remove("BGM");
					callBack->OnSceneChange(SceneName::Game, nullptr, SceneStack::AllClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			else if (b->HasComponent<ECS::ContinueButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					//ポーズボタン生成
					ECS::Entity* pauseBtn = ECS::ButtonArcheType()("pauseButton", Vec2(1280 - 96, 0), Vec2(0, 0), Vec2(96, 96), 50);
					pauseBtn->AddComponent<ECS::PauseButtonTag>();
					pauseBtn->AddGroup(ENTITY_GROUP::GameUI);
					CommonData::CurrentScene::val = Scene::SceneName::Game;
					callBack->OnSceneChange(SceneName::BackToScene, nullptr, SceneStack::OneClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			else if (b->HasComponent<ECS::RetryButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					ResourceManager::GetSound().Remove("BGM");
					callBack->OnSceneChange(SceneName::Menu, nullptr, SceneStack::AllClear);
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
		}
	}
	void Pause::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}