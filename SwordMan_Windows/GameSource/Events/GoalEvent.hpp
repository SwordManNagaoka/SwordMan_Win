//----------------------------------------------------
//!@file	GoalEvent.hpp
//!@brief	ゴール時のイベントを作成します
//!@author	日比野　真聖
//!@date	2018/10/10
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
#include "../Collision/Collision.hpp"
#include "../Components/BasicComponents.hpp"
#include "../System/System.hpp"
#include "../Components/Think.hpp"
#include "../Components/Behavior.hpp"
#include "../Components/PlayerDash.hpp"
#include "../Components/EasingMove.hpp"


namespace Event
{
	class GoalEvent
	{
	public:
		//プレイヤーと衝突したときのゴールの処理
		static void HitPlayer()
		{
			const auto& players = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (players.size() == 0) { return; }
			const auto& events = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Back3);
			if (events.size() == 0) { return; }
			for (auto& e : events)
			{
				if (Collision::BoxAndBox<ECS::HitBase, ECS::HitBase>(*players[0], *e))
				{
					//ゲームクリア
					//プレイヤーが右に走り抜けていく
					if (!players[0]->HasComponent<ECS::PlayerDash>())
					{
						players[0]->AddComponent<ECS::PlayerDash>().SetTargetPos(Vec2(1380, 0));
						players[0]->GetComponent<ECS::PlayerDash>().SetAddSpeed(0.2f);
						CreateGoalCharacter();
					}
					players[0]->DeleteComponent<ECS::Think>();
					players[0]->DeleteComponent<ECS::Behavior>();
					break;
				}
			}
		}
		//プレイヤーが動作を終了したら、ゴール文字を出現させる
		static void AppearGoalCharacter()
		{
			const auto& players = ECS::EcsSystem().GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (players.size() == 0) { return; }
			if (!players[0]->HasComponent<ECS::PlayerDash>()) { return; }
			if (players[0]->GetComponent<ECS::PlayerDash>().IsTargetPos())
			{
				//プレイヤーを消去
				players[0]->Destroy();
				//Scene::SceneManager::Get().ChangeScene(Scene::SceneManager::State::Result);
			}
		}
	public:
		//ゴール文字の作成
		static void CreateGoalCharacter()
		{
			for (int i = 0; i < 4; ++i)
			{
				ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
				entity->AddComponent<ECS::Position>(Vec2(1300 + i* 170, 200));
				entity->AddComponent<ECS::RectDraw>("goalMessage",i * 180,0,180,210);
				entity->AddComponent<ECS::EasingMove>(Easing::QuartOut, ECS::EasingMove::DirectionState::LeftAndRight);
				entity->GetComponent<ECS::EasingMove>().SetBeginToEndPoint(1300 + i * 170, 350 + i * 170);
				entity->GetComponent<ECS::EasingMove>().SetTimeToDuration(30, 60);
				entity->AddGroup(ENTITY_GROUP::GameUI);
			}
		}
	};
}