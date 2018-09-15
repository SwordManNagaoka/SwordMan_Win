/**
* @file Map.hpp
* @brief Mapの原型作成します。
* @author tonarinohito
* @date 2018/9/14
*/
#pragma once
#include "../ECS/ECS.hpp"
#include "../Components/Collider.hpp"
#include "../Components/Renderer.hpp"
#include "../GameController/Game.h"
#include "../Components/MapMove.hpp"
namespace ECS
{
	//マップチップ一つあたりの大きさは96x96の予定
	class MapArcheType : public IArcheType<const char* ,const Vec2&, const Vec2&, const int, const int, const int>
	{
	public:
		ECS::Entity* operator()(const char* name, const Vec2& pos, const Vec2& velocity, const int w, const int h, const int id) override
		{
			ECS::Entity* entity = &ECS::EcsSystem::GetManager().AddEntity();
			switch (id)
			{
			case -1:
				break;
			case 0: //地面
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 0, 0, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			case 1: //断面
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 96, 0, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			case 2: //地面左端
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 0, 96, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			case 3: //断面左端
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 96, 96, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			case 4: //地面右端
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 0, 192, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			case 5: //断面右端
				entity->AddComponent<Position>(pos);
				entity->AddComponent<Velocity>(velocity);
				entity->AddComponent<MapMove>();
				entity->AddComponent<HitBase>(static_cast<float>(w), static_cast<float>(h)).SetColor(255, 0, 0);
				entity->AddComponent<RectDraw>(name, 96, 192, w, h);
				entity->AddGroup(ENTITY_GROUP::Ground);
				break;
			}
			return entity;
		}
	};
}