/**
* @file ArcheType.hpp
* @brief 汎用的な原型を作ります
* @author tonarinohito
* @date 2018/12/20
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/MapMove.hpp"
#include "../../Utility/Random.hpp"
namespace ECS
{
	class ArcheType : public IArcheType<const char*,const Vec2&, const Group&>
	{
	public:
		ECS::Entity* operator()(const char* name,const Vec2& pos,const Group& group)
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			entity->AddComponent<Transform>().SetPosition(pos.x,pos.y);
			entity->AddComponent<Color>();
			entity->AddComponent<AlphaBlend>();
			entity->AddComponent<SimpleDraw>(name);
			entity->AddGroup(group);
			return entity;
		}
	};
}