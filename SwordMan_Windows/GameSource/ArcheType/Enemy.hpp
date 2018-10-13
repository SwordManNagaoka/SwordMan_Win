//----------------------------------------------------
//!@file	Enemy.hpp
//!@brief	敵の原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/14
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/Jump.hpp"
#include "../Components/Think.hpp"
#include "../Components/Behavior.hpp"
#include "../Components/AnimationController.hpp"
#include "../Components/EnemyDefaultMove.hpp"
#include "../Components/ComponentDatas/EnemyData.hpp"
#include "../Components/EnemyJumpMove.hpp"

#include "../Components/PlayerDash.hpp"
#include "../ArcheType/Score.hpp"
#include "../Events/GoalEvent.hpp"

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
				players[0]->GetComponent<ECS::PlayerDash>().SetIsMove(true);
				players[0]->GetComponent<ECS::PlayerDash>().SetTargetPos(Vec2(1300, 0));
				players[0]->GetComponent<ECS::PlayerDash>().SetAddSpeed(0.2f);
				players[0]->DeleteComponent<ECS::Think>();
				players[0]->DeleteComponent<ECS::Behavior>();
				if (isEventFunc)
				{
					if (onlyone)
					{
						if (!onceReferrd)
						{
							onceReferrd = true;
							eventFunc();
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
		for (int i = 0; i < 4; ++i)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<ECS::Position>(Vec2(1300 + i * 170, 200));
			entity->AddComponent<ECS::RectDraw>("goalMessage", i * 180, 0, 180, 210);
			entity->AddComponent<ECS::EasingMove>(Easing::QuartOut, ECS::EasingMove::DirectionState::LeftAndRight);
			entity->GetComponent<ECS::EasingMove>().SetBeginToEndPoint(1300 + i * 170, 350 + i * 170);
			entity->GetComponent<ECS::EasingMove>().SetTimeToDuration(30, 60);
			entity->AddGroup(ENTITY_GROUP::GameUI);
		}
	}
	
	class EnemyCommonArcheType : public IArcheType<const EnemyCommonData&,const int>
	{
	public:
		ECS::Entity* operator()(const EnemyCommonData& data,const int id)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntityAddTag("Enemy");
			entity->AddComponent<Position>(data.pos);
			entity->AddComponent<Velocity>().val.x = data.moveSpeed;
			entity->AddComponent<HitBase>(data.size.x,data.size.y);
			entity->AddComponent<EnemyDefaultMove>();
			entity->AddComponent<Direction>();
			entity->AddComponent<AnimationDraw>(data.imageName.c_str());
			entity->AddComponent<AnimationController>(data.changeAnimFrameTime, data.animNum);

			switch (id)
			{
			case 0:	//青鎧
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 1: //赤
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 2: //緑バネ
				entity->AddComponent<Physics>();
				entity->AddComponent<TriggerJumpMove>(data.jumpPower);
				entity->AddComponent<EnemyJumpMove>(data.changeAnimFrameTime * 2);
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 3: //水色羽
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 4: //ゴール
				entity->AddComponent<GoalEventHitPlayer>().SetEventFunction(ECS::CreateGoalCharacter);
				entity->AddGroup(ENTITY_GROUP::Back3);
				break;
			}
			return entity;
		}
	};
}