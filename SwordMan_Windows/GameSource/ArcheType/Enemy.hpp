//----------------------------------------------------
//!@file	Enemy.hpp
//!@brief	敵の原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/14
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../GameController/GameController.h"
//アーキタイプ
#include "../ArcheType/Score.hpp"
//コンポーネント
#include "../Components/Jump.hpp"
#include "../Components/Think.hpp"
#include "../Components/Behavior.hpp"
#include "../Components/AnimationController.hpp"
#include "../Components/EnemyDefaultMove.hpp"
#include "../Components/ComponentDatas/EnemyData.hpp"
#include "../Components/EnemyJumpMove.hpp"
#include "../Components/PlayerDash.hpp"
#include "../Components/GoalEventHitPlayer.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"

namespace ECS
{
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
				entity->AddComponent<Physics>();
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 2: //緑バネ
				entity->AddComponent<Physics>();
				entity->AddComponent<TriggerJumpMove>(-data.jumpPower);
				entity->AddComponent<EnemyJumpMove>(data.changeAnimFrameTime);
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 3: //水色羽
				entity->AddGroup(ENTITY_GROUP::Enemy);
				break;
			case 4: //ゴール
				entity->GetComponent<HitBase>().SetOffset(0, -entity->GetComponent<HitBase>().h() / 2.0f);
				entity->AddComponent<GoalEventHitPlayer>().SetEventFunction(ECS::CreateGoalCharacter);
				entity->AddGroup(ENTITY_GROUP::Back3);
				break;
			}
			return entity;
		}
	};
}