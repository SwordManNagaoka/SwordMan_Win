#include "Game.h"
#include "../../Events/AtackEvent.hpp"
#include "../../GameController/GameController.h"
#include "../../ECS/ECS.hpp"
#include "../../ArcheType/Map.hpp"
#include "../../ArcheType/Player.hpp"
#include "../../Events/EventManager.hpp"
#include "../../Class/Scene/SceneManager.hpp"
#include "../../Class/TouchInput.hpp"
#include "../../ArcheType/Enemy.hpp"
#include "../../ArcheType/HealthUI.hpp"
#include "../../ArcheType/Score.hpp"
#include "../../ArcheType/Button.hpp"
#include "../../Events/AtackEvent.hpp"
#include "../../Utility/Input.hpp"

namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange,[[maybe_unused]] Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{

		switch (CommonData::StageNum::val)
		{
		case 1:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam01.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam01.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif
			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage1";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);
			break;
		}
		case 2:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam02.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam02.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif
			
			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage2";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);
			break;
		}
		case 3:
		{
#ifdef __ANDROID__
			// Android版のコンパイル
			stageLoader.LoadStage("stage/stageparam03.csv");
			ResourceManager::GetSound().Load("sounds/nagaoka.wav", "BGM", SoundType::BGM);
#else
			// Windows版のコンパイル
			stageLoader.LoadStage("Resource/stage/stageparam03.csv");
			ResourceManager::GetSound().Load("Resource/sounds/nagaoka.wav", "BGM", SoundType::BGM);
#endif
			stageLoader.LoadStageConstitution();
			const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = "stage3";
			stageCreator.SetMapParam(stageLoader.GetStageParam());
			stageCreator.FillUpFlatMap();
			Sound s("BGM");
			s.Play(true, false);
			break;
		}
		default:
			break;
		}

		//Entityの生成
		ECS::PlayerArcheType()(Vec2(-150.f, 300.f), Vec2(64, 96));
		for (float i = 0; i < 3; ++i)
		{
			ECS::HealthUIArcheType()(i, Vec2(450.f + i * 144.f, 640.f));
		}
		//トータルスコアの生成
		ECS::TotalScoreArcheType()("font", Vec2(0, 0));
		//ポーズボタン生成
		ECS::Entity* pauseBtn = ECS::ButtonArcheType()("pauseButton", Vec2(1280.f - 96.f, 0.f), Vec2(0.f, 0.f), Vec2(96.f, 96.f), 50.f);
		pauseBtn->AddComponent<ECS::PauseButtonTag>();
		pauseBtn->AddGroup(ENTITY_GROUP::GameUI);
#ifdef __ANDROID__
		// Android版のコンパイル
		//非アクティブ時にポーズ画面に移行する
		SetAndroidLostFocusCallbackFunction([](void* ptr)
		{
			auto callback = static_cast<IOnSceneChangeCallback*>(ptr);
			if (CommonData::CurrentScene::val == SceneName::Game)
			{
				callback->OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
				return;
			}
		},
			&GetCallback());
#endif
		
	}
	Game::~Game()
	{
		ECS::EcsSystem::GetManager().AllKill();
	}

	void Game::Update()
	{
		cloud.Run();
		stageCreator.Run(&stageLoader.GetStageData(), &stageLoader.GetSkyData(), &stageLoader.GetEnemyData());
		auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
		auto& ground = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground);
		auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		//まれにめり込んだ状態から始まり、ジャンプできなくなるので苦肉の策としてこうしてある
		if (!isIntrusion)
		{
			for (const auto& p : player)
			{
				for (const auto& g : ground)
				{
					if (Collision::BoxAndBox<ECS::FootBase, ECS::HitBase>(*p, *g))
					{
						p->GetComponent<ECS::Position>().val.y -= 60;
						isIntrusion = true;
					}
				}
			}
		}
		//地形との衝突応答を行う
		for (const auto& p : player)
		{
			p->GetComponent<ECS::Physics>().PushOutEntity(ground);
			p->GetComponent<ECS::Physics>().SetCollisionFunction(Collision::BoxAndBox<ECS::FootBase, ECS::HitBase>);
		}
		auto& enemy = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy);
		for (const auto& e : enemy)
		{
			if (e->HasComponent<ECS::Physics>())
			{
				e->GetComponent<ECS::Physics>().PushOutEntity(ground);
				e->GetComponent<ECS::Physics>().SetCollisionFunction(Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>);
			}
		}
		Event::CollisionEvent::AttackCollisionToEnemy();
		Event::CollisionEvent::PlayerToEnemy();
		ECS::EcsSystem::GetManager().Update();
		
		//キーイベント(仮)
		if (Input::Get().GetKeyFrame(KEY_INPUT_SPACE) == 1)
		{
			GetCallback().OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
			return;
		}
		//ボタンイベント
		for (auto& b : gameUI)
		{
			if (b->HasComponent<ECS::PauseButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(&GetCallback());
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					callBack->OnSceneChange(SceneName::Pause, nullptr, SceneStack::Non);
					auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
					for (auto& b : gameUI)
					{
						if (b->HasComponent<ECS::PauseButtonTag>()) { b->Destroy(); }
					}
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			//スコア
			for (auto& ui : gameUI)
			{
				if (ui->HasComponent<ECS::TotalScoreDraw>())
				{
					CommonData::TotalScore::val = ui->GetComponent<ECS::TotalScoreDraw>().GetTotalScore();
				}
			}
		}
		//プレイヤー死亡時
		if (!player[0]->IsActive() && !player.empty())
		{
			for (auto& ui : gameUI)
			{
				if (ui->HasComponent<ECS::PauseButtonTag>())
				{
					ui->Destroy();
				}
				else if (ui->HasComponent<ECS::TotalScoreDraw>())
				{
					CommonData::TotalScore::val = ui->GetComponent<ECS::TotalScoreDraw>().GetTotalScore();
				}
			}
#ifdef __ANDROID__
			ResourceManager::GetGraph().LoadDiv("image/death.png", "death", 2, 2, 1, 96, 192);
#else
			ResourceManager::GetGraph().LoadDiv("Resource/image/death.png", "death", 2, 2, 1, 96, 192);
#endif
			ECS::PlayerDeathArcheType()("death", Vec2{ player[0]->GetComponent<ECS::Position>().val });
			GetCallback().OnSceneChange(SceneName::Result, nullptr, SceneStack::Non);
			return;
		}
	}
	void Game::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}
