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


namespace Scene
{
	Game::Game(IOnSceneChangeCallback* sceneTitleChange, Parameter* parame)
		: AbstractScene(sceneTitleChange)
	{
		auto a = parame->Get<const char*>("stagePath");
		a;
		stageLoader.LoadStage(parame->Get<const char*>("stagePath"));
		stageLoader.LoadStageConstitution();
		const_cast<StageParam&>(stageLoader.GetStageParam()).mapImage = parame->Get<const char*>("stageNum");
		stageCreator.SetMapParam(stageLoader.GetStageParam());
		stageCreator.FillUpFlatMap();
		
		//ステージの生成
		stageCreator.Run(&stageLoader.GetStageData(), &stageLoader.GetSkyData(),&stageLoader.GetEnemyData());
		//Entityの生成
		ECS::PlayerArcheType()(Vec2(-150, 300), Vec2(64, 96));
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
		ECS::EcsSystem::GetManager().Update();
		//ボタンイベント
		auto& gameUI = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI);
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
			else if (player.size() == 0)
			{
				auto param = std::make_unique<Parameter>();
				for (auto& ui : gameUI)
				{
					if(ui->HasComponent<ECS::PauseButtonTag>())
					{
						ui->Destroy();
					}
					else if (ui->HasComponent<ECS::TotalScoreDraw>())
					{
						param->Set<int>("score", ui->GetComponent<ECS::TotalScoreDraw>().GetTotalScore());
					}
				}
				GetCallback().OnSceneChange(SceneName::Result, param.get(), SceneStack::Non);
				return;
			}
		}
	}
	void Game::Draw()
	{
		ECS::EcsSystem::GetManager().OrderByDraw(ENTITY_GROUP::Max);
		DrawFormatString(0, 120, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Ground).size());
		DrawFormatString(0, 140, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back0).size());
		DrawFormatString(0, 160, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back1).size());
		DrawFormatString(0, 180, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back2).size());
		DrawFormatString(0, 200, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back3).size());
		DrawFormatString(0, 220, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Gimmick).size());
		DrawFormatString(0, 240, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Wepon).size());
		DrawFormatString(0, 260, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player).size());
		DrawFormatString(0, 280, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Enemy).size());
		DrawFormatString(0, 300, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Effect).size());
		DrawFormatString(0, 320, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::GameUI).size());
		DrawFormatString(0, 340, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade1).size());
		DrawFormatString(0, 360, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::PauseUI).size());
		DrawFormatString(0, 380, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Fade2).size());
		DrawFormatString(0, 400, 0xffffffff, "%d", ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Max).size());
	}
}
