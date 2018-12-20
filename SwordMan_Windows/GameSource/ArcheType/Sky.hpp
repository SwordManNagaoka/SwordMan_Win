/**
* @file Sky.hpp
* @brief 背景(空)の原型を作成する
* @author feveleK5563
* @date 2018/10/8
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/GameController.h"
#include "../Components/MapMove.hpp"
namespace ECS
{
	//マップチップ一つあたりの大きさは96x96
	class SkyArcheType : public IArcheType<const char*, const Vec2&, const Vec2&, const int, const int, const int>
	{
	public:
		ECS::Entity* operator()(const char* imageName, const Vec2& pos, const Vec2& velocity, const int w, const int h, const int id) override
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();

			entity->AddComponent<Position>(pos);
			entity->AddComponent<Velocity>(velocity);
			entity->AddComponent<MapMove>();
			entity->AddComponent<RectDraw>(imageName, (id % 11) * w, (id / 11) * h, w, h);
			entity->AddGroup(ENTITY_GROUP::Back1);

			return entity;
		}
	};
}