//----------------------------------------------------
//!@file	Health.hpp
//!@brief	プレイヤーの体力のUI原型を作成します
//!@author	日比野　真聖
//!@date	2018/9/26
//----------------------------------------------------
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/HealthRelationUI.hpp"

namespace ECS
{
	class HealthUIArcheType : public IArcheType<const int, const Vec2&>
	{
	public:
		ECS::Entity* operator()(const int id, const Vec2& pos)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Position>(pos);
			entity->AddComponent<HealthRelationUI>(id);
			entity->AddComponent<Direction>();
			entity->AddComponent<SimpleDraw>("health");
			entity->AddGroup(ENTITY_GROUP::GameUI);
			return entity;
		}
	};
}