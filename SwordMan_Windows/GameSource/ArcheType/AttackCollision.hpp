//----------------------------------------------------
//!@file	Player.hpp
//!@brief	プレイヤーの原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/14
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/AnimationController.hpp"
#include "../Components/ChaseEntity.hpp"


namespace ECS
{
	class SwordAttackCollision : public IArcheType<const Vec2&, const Vec2&,const int>
	{
	public:
		ECS::Entity* operator()(const Vec2& pos, const Vec2& size,const int killTime)
		{
			const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (player.size() == 0) { return nullptr; }

			ECS::Entity*	entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<ECS::Position>(pos);
			entity->AddComponent<ECS::ChaseEntity>(*player[0]);
			entity->AddComponent<ECS::HitBase>(size.x,size.y).SetColor(0, 255, 0);
			entity->GetComponent<ECS::HitBase>().SetOffset(size.x, 0.0f);
			entity->AddComponent<ECS::KillEntity>(killTime);
			entity->AddComponent<ECS::Direction>();
			entity->AddComponent<ECS::AnimationDraw>("sword").Offset(Vec2(-12.0f, -84.0f));
			entity->AddComponent<ECS::AnimationController>(4, 5);
			entity->AddGroup(ENTITY_GROUP::Wepon);
			return entity;
		}
	};


	class JumpAttackCollision : public IArcheType<const Vec2&, const Vec2&,const int>
	{
	public:
		ECS::Entity* operator()(const Vec2& pos, const Vec2& size,const int killTime)
		{
			const auto& player = ECS::EcsSystem::GetManager().GetEntitiesByGroup(ENTITY_GROUP::Player);
			if (player.size() == 0) { return nullptr; }

			ECS::Entity*	entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<ECS::Position>(pos);
			entity->AddComponent<ECS::ChaseEntity>(*player[0]);
			entity->AddComponent<ECS::HitBase>(size.x,size.y).SetColor(128, 0, 0);
			entity->GetComponent<ECS::HitBase>().SetOffset(-size.x / 3.0f, -size.y / 3.0f);
			entity->AddComponent<ECS::KillEntity>(killTime);
			entity->AddGroup(ENTITY_GROUP::Wepon);
			return entity;
		}
	};
}

;