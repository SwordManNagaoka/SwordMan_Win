#include "Game.h"
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
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, const Parameter& parame)
		: AbstractScene(sceneTitleChange)
	{
		stageLoader.LoadStage("Resource/stage/mapparamtest.csv");
		stageLoader.LoadStageConstitution();
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		//ステージの生成
		stageCreator.Run(&stageLoader.GetStageData(), &stageLoader.GetSkyData(),&stageLoader.GetEnemyData());
		//Entityの生成
		//ECS::PlayerArcheType()(Vec2(250, 300), Vec2(64, 96));
		for (int i = 0; i < 3; ++i)
		{
			ECS::HealthUIArcheType()(i,Vec2(450 + i * 144, 640));
		}
		//トータルスコアの生成
		ECS::TotalScoreArcheType()("font", Vec2(0, 0));
		//ポーズボタン生成
		ECS::Entity* pauseBtn = ECS::ButtonArcheType()("pauseButton", Vec2(1280 - 96, 0), Vec2(0, 0), Vec2(96, 96), 50);
		pauseBtn->AddComponent<ECS::PauseButtonTag>();
		pauseBtn->AddGroup(ENTITY_GROUP::GameUI);
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
		//地形との衝突応答を行う
		for (const auto& p : player)
		{
			p->GetComponent<ECS::Physics>().PushOutEntity(ground);
			p->GetComponent<ECS::Physics>().SetCollisionFunction(Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>);
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
		//ボタンイベント
		auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
		/*for (auto& b : gameUI)
		{
			if (b->HasComponent<ECS::PauseButtonTag>())
			{
				b->GetComponent<ECS::PushButton>().SetSceneCallBack(callBack);
				auto changeFunc = [](Scene::IOnSceneChangeCallback* callBack)
				{
					Parameter param;
					callBack->OnSceneChange(SceneName::Pause, param, false);
					auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
					for (auto& b : gameUI)
					{
						if (b->HasComponent<ECS::PauseButtonTag>()) { b->Destroy(); }
					}
					return;
				};
				b->GetComponent<ECS::PushButton>().SetEventFunction(changeFunc);
			}
			else if (player.size() == 0)
			{
				for (auto& ui : gameUI)
				{
					if(ui->HasComponent<ECS::PauseButtonTag>())
					{
						ui->Destroy();
					}
				}
				Parameter param;
				callBack->OnSceneChange(SceneName::Result, param, false);
				return;
			}
		}*/
		if (Input::Get().GetKeyFrame(KEY_INPUT_S) == 1)
		{
			Parameter param;
			callBack->OnSceneChange(SceneName::Pause, param, SceneStack::Non);
			auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
			for (auto& b : gameUI)
			{
				if (b->HasComponent<ECS::PauseButtonTag>()) { b->Destroy(); }
			}
			return;
		}
		else if (player.size() == 0)
		{
			for (auto& ui : gameUI)
			{
				if (ui->HasComponent<ECS::PauseButtonTag>())
				{
					ui->Destroy();
				}
			}
			Parameter param;
			callBack->OnSceneChange(SceneName::Result, param, SceneStack::Non);
			return;
		}
	}
	void Game::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
	}
}
