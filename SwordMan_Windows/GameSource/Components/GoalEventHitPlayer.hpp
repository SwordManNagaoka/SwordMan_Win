//----------------------------------------------------
//!@file	GoalEventHitPlayer.hpp
//!@brief	ゴールとプレイヤーが衝突したときのプレイヤーの動作を作成します
//!@author	日比野　真聖
//!@date	2018/10/13
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
//コンポーネント
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/ComponentDatas/EnemyData.hpp"
#include "../Components/EnemyJumpMove.hpp"
#include "../Components/PlayerDash.hpp"
#include "../Components/EasingMove.hpp"

namespace ECS
{
	class GoalEventHitPlayer : public Component
	{
	public:
		void Initialize() override
		{
			collision = &entity->GetComponent<ECS::HitBase>();
		}
		void Update() override
		{
			const auto& players = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (players.size() == 0) { return; }

			if (Collision::BoxAndBox<HitBase, HitBase>(*entity, *players[0]))
			{
				//当たり判定時のイベント
				if (isEventFunc)
				{
					if (onlyone)
					{
						if (!onceReferrd)
						{
							players[0]->GetComponent<Think>().ChangeMotion(PlayerData::State::Walk);
							players[0]->GetComponent<ECS::PlayerDash>().SetIsMove(true);
							players[0]->GetComponent<ECS::PlayerDash>().SetTargetPos(Vec2(1300, 0));
							players[0]->GetComponent<ECS::PlayerDash>().SetAddSpeed(0.2f);
							players[0]->DeleteComponent<ECS::Think>();
							players[0]->DeleteComponent<ECS::Behavior>();
							eventFunc();
							onceReferrd = true;
						}
					}
					else
					{
						eventFunc();
					}
				}
			}
		}
		void Draw2D() override {}
		//イベント関数を設定する
		//引数1: イベント関数のポインタ
		//引数2: true:1回だけイベント実行 false:複数回イベント実行
		void SetEventFunction(const std::function<void()>& eventFunction, bool isOnlyone = true)
		{
			eventFunc = eventFunction;
			isEventFunc = true;
			onlyone = isOnlyone;
		}
	private:
		void Draw3D() override {}
	private:
		HitBase* collision;
		std::function<void()> eventFunc;
		bool isEventFunc = false;
		bool onceReferrd = false;
		bool onlyone = false;
	};
	static void CreateGoalCharacter()
	{
		for (float i = 0; i < 4; ++i)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<ECS::Position>(Vec2(1300 + i * 170, 200));
			entity->AddComponent<ECS::RectDraw>("goalMessage", i * 180, 0, 180, 210);
			entity->AddComponent<ECS::EasingMove>(Easing::QuartOut, ECS::EasingMove::DirectionState::LeftAndRight);
			entity->GetComponent<ECS::EasingMove>().SetBeginToEndPoint(1300 + i * 170, 320 + i * 170);
			entity->GetComponent<ECS::EasingMove>().SetTimeToDuration(30, 60);
			entity->AddGroup(ENTITY_GROUP::GameUI);
		}
	}
}